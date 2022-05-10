#include "Character.hpp"
#include <src/physics/rigid_body_behavior/MovementBehavior.hpp>

Character::Character(MainShaders* shaders, ElementSG* parent, PhysicsSystem* phi){
    m_physics = phi;
    m_scene_root = parent;
    m_item = nullptr;
    m_has_item = false;
    m_power = POWER_INIT;
    m_act_timestamp = 0;

    auto* body_node = new NodeGameSG(parent, OBB_TYPE);

    Mesh* body_mesh = new Mesh(create_rectangle_cuboid({2,8,2}), true, OBB_TYPE);
    Mesh* arm_mesh = new Mesh("../assets/mesh/arm_3_3.obj", true, OBB_TYPE);

    auto* material_character = new MaterialColor(shaders, {0.8,0.8,0.8}, 100);
    
    body_node->get_trsf()->set_translation({2.,4,0});
    body_node->get_trsf()->set_translation({-20, 10, 0});
//    body_node->set_debug_rendering(true, {0, 0.5, 0.7});
    body_node->set_meshes({body_mesh});


    //Initial camera (FPS)
    m_camera = new NodeGameSG(body_node, OBB_TYPE);
   
    glm::vec3 pos;
    m_camera->get_trsf()->set_translation({0, 5, 0});
    // m_camera->set_debug_rendering(true, {0.6, 0, 0.7});
    // m_camera->get_trsf()->set_scale({0.3, 0.3, 0.3});
    // m_camera->get_trsf()->set_rotation({-10, 0, 0});
    
    //Arm mesh 
    auto* arm_node = new NodeGameSG(m_camera, OBB_TYPE);
    arm_node->set_meshes({arm_mesh});
    arm_node->set_material(material_character);

   arm_node->get_trsf()->set_uniform_scale(0.4f);
   arm_node->get_trsf()->set_order_rotation(ORDER_ZXY);
   arm_node->get_trsf()->set_rotation({20, -100, 30});

    arm_node->get_trsf()->set_translation({1.7, -1.0, -2.5});
    // arm_node->set_debug_rendering(true, {0.6, 0, 0.7});


    m_body = new RigidBodyVolume(body_node, true);
    m_body->add_behavior(new MovementBehavior(true,false,10.f, 0.6f, 0.5f));
    m_body->get_movement_behavior()->add_force(new GravityForce());

    //MOUSE EVENT
    m_mouse_view = MouseView::get_instance();


}

NodeGameSG* Character::get_camera(){
    return m_camera;
}

NodeGameSG* Character::get_character_node(){
    return m_body->get_node();
}

glm::vec3 Character::get_sight(){
    glm::vec3 fwd = NODE_INIT_FORWARD;
    return m_camera->get_trsf()->apply_to_vector(fwd);
}

void Character::grab_item(RigidBodyVolume* item, double ts, float action_area){
    
    glm::vec3 item_pos = item->get_node()->get_trsf()->get_translation();
    float distance = glm::length(item_pos - m_body->get_node()->get_trsf()->get_translation());
    
    if(distance < action_area && item->has_movement_behavior()){
        m_act_timestamp = ts;

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


        // glm::vec3 item_translation = m_camera->get_trsf()->apply_to_vector(fwd);
        m_item->get_node()->get_trsf()->set_translation(trsf);

        m_has_item = true;
    }
}


RigidBodyVolume* Character::get_item(){
    return m_item;
}

RigidBodyVolume* Character::get_body(){
    return m_body;
}

void Character::throw_item(double ts){
    m_act_timestamp = ts;
    m_has_item = false;

    glm::vec3 fwd = NODE_INIT_FORWARD;
    glm::vec3 dir = m_camera->get_trsf()->apply_to_vector(fwd);
    glm::vec3 throw_dir = m_power * dir;
    
    m_power = POWER_INIT;
    
    RigidBodyVolume* item = m_item;
    m_item = nullptr;
    m_camera->get_children()[0]->clear_children();


    //Get closest pt to item pos on character boundingbox
    glm::vec3 pos = item->get_node()->get_position_in_world();
    glm::vec3 bb_pt = get_character_node()->get_bb()->closest_point(pos);

    //Add item back to physics
    m_physics->add_collider(item);
    item->get_node()->set_parent(m_scene_root);
    m_scene_root->add_child(item->get_node());
    
    item->get_node()->get_trsf()->set_translation(bb_pt);

    //Add force back and throw item.
    item->get_movement_behavior()->add_linear_impulse(throw_dir);
    item->get_movement_behavior()->add_force(new GravityForce());
    
}

void Character::jump(){
    // m_body->get_movement_behavior()->clear_forces();
    
    glm::vec3 vertical = glm::vec3(0, 1, 0);
    vertical *= Character::HEIGHT_JUMP;
    m_body->get_movement_behavior()->set_velocity(vertical);
    // m_body->get_movement_behavior()->add_force(new GravityForce());

}

void Character::accumulate_power(){
    m_power += INCREASE_POWER;
}

bool Character::can_interact(double timestamp) const{
    return (timestamp - m_act_timestamp) > 0.5;
}

MouseView *Character::get_mouse_view()const {
    return m_mouse_view;
}

void Character::set_mouse_view(MouseView *mouse_view) {
    m_mouse_view = mouse_view;
}

bool Character::has_item() {
    return m_has_item;
}







