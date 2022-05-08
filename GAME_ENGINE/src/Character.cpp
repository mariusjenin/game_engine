#include "Character.hpp"
#include <src/physics/rigid_body_behavior/MovementBehavior.hpp>

Character::Character(Shaders* shaders, ElementSG* parent, PhysicsSystem* phi){
    m_physics = phi;
    m_scene_root = parent;
    m_item = nullptr;
    m_power = 1.f;

    auto* body_node = new NodeGameSG(shaders, parent, OBB_TYPE);

    Mesh* body_mesh = new Mesh(create_rectangle_cuboid({2,8,2}), true, OBB_TYPE);
    Mesh* arm_mesh = new Mesh("../assets/mesh/arm_3_3.obj", true, OBB_TYPE);

    auto* material_character = new MaterialColor(shaders, {0.8,0.8,0.8}, 100);
    
    body_node->get_trsf()->set_translation({2.,4,0});
    body_node->get_trsf()->set_translation({-20, 10, 0});
    body_node->set_debug_rendering(true, {0, 0.5, 0.7});
    body_node->set_meshes({body_mesh});


    //Initial camera (FPS)
    m_camera = new NodeGameSG(shaders, body_node, OBB_TYPE);
   
    glm::vec3 pos;
    m_camera->get_trsf()->set_translation({0, 5, 0});
    // m_camera->set_debug_rendering(true, {0.6, 0, 0.7});
    // m_camera->get_trsf()->set_scale({0.3, 0.3, 0.3});
    // m_camera->get_trsf()->set_rotation({-10, 0, 0});
    
    //Arm mesh 
    auto* arm_node = new NodeGameSG(shaders, m_camera, OBB_TYPE);
    arm_node->set_meshes({arm_mesh});
    arm_node->set_material(material_character);

//   TODO use these to have the arm placed better
   arm_node->get_trsf()->set_uniform_scale(0.5f);
   arm_node->get_trsf()->set_order_rotation(ORDER_ZXY);
   arm_node->get_trsf()->set_rotation({20, -90, 35});

    // arm_node->get_trsf()->set_rotation({0, -90, 0});
    arm_node->get_trsf()->set_translation({1.2, -1.2, -2.1});
    // arm_node->set_debug_rendering(true, {0.6, 0, 0.7});


    m_body = new RigidBodyVolume(body_node, true);
    m_body->add_behavior(new MovementBehavior(true,false,10.f, 0.6f, 0.5f));
    m_body->get_movement_behavior()->add_force(new GravityForce());

    m_sight = CAMERA_INIT_FORWARD;


    //MOUSE EVENT
    m_mouse_view = MouseView::get_instance();


}

NodeGameSG* Character::get_camera(){
    return m_camera;
}

NodeGameSG* Character::get_character_node(){
    return m_body->get_node();
}

void Character::set_camera(NodeGameSG* cam){
    m_camera = cam;
    m_body->get_node()->add_child(cam);
}

PhysicsSystem* Character::get_physics(){
    return m_physics;
}

void Character::set_physics(PhysicsSystem* system){
    m_physics = system;
}


glm::vec3 Character::get_sight(){
    glm::vec3 fwd = CAMERA_INIT_FORWARD;
    return m_camera->get_trsf()->apply_to_vector(fwd);
}

void Character::grab_item(RigidBodyVolume* item, float action_area){
    
    glm::vec3 item_pos = item->get_node()->get_trsf()->get_translation();
    float distance = glm::length(item_pos - m_body->get_node()->get_trsf()->get_translation());
    if(distance < action_area && item->has_movement_behavior()){
        //ITEM can be grabbed
        m_item = item;
        m_item->get_movement_behavior()->clear_forces();

        //Item must be relative to camera
        m_item->get_node()->set_parent(m_camera->get_children()[0]);
        m_camera->get_children()[0]->add_child(m_item->get_node());

        //Remove item from physics system
        m_physics->remove_collider(m_item);

        glm::vec3 trsf(-3., 2.5, 0);
        // glm::vec3 translation_wrld(1.2, -1.2, -2.1);

        // translation_wrld += fwd;

        // glm::vec3 item_translation = m_camera->get_trsf()->apply_to_vector(fwd);
        m_item->get_node()->get_trsf()->set_translation(trsf);
    }
}

bool Character::has_item(){
    return m_item != nullptr;
}

RigidBodyVolume* Character::get_item(){
    return m_item;
}

RigidBodyVolume* Character::get_body(){
    return m_body;
}


//Keep item in front of camera when moving mouse
void Character::update_item(){
//     glm::vec3 fwd = 4.f * CAMERA_INIT_FORWARD;
//     glm::vec3 pos = m_camera->get_trsf()->apply_to_vector(fwd);
//     m_item->get_node()->get_trsf()->set_translation(pos);
}

void Character::throw_item(){
    glm::vec3 fwd = CAMERA_INIT_FORWARD;
    glm::vec3 dir = m_camera->get_trsf()->apply_to_vector(fwd);
    glm::vec3 throw_dir = m_power * dir;
    m_power = 0.f;
    RigidBodyVolume* item = m_item;
    m_item = nullptr;

    m_camera->get_children()[0]->clear_children();

    //save worldpos before getting new parent
    glm::vec3 pos = item->get_node()->get_position_in_world();

    m_physics->add_collider(item);
    
    item->get_node()->set_parent(m_scene_root);
    item->get_node()->get_trsf()->set_translation(pos);

    //Add force back and throw item.
    item->get_movement_behavior()->add_linear_impulse(throw_dir);
    item->get_movement_behavior()->add_force(new GravityForce());
    
}

void Character::jump(){
    float height_fact = 9.f;
    // m_body->get_movement_behavior()->clear_forces();
    
    glm::vec3 vertical = height_fact*glm::vec3(0, 1, 0);
    m_body->get_movement_behavior()->set_velocity(vertical);
    // m_body->get_movement_behavior()->add_force(new GravityForce());

}

void Character::accumulate_power(){
    m_power += 0.2f;
}






