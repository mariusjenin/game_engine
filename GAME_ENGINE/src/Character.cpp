#include "Character.hpp"

Character::Character(Shaders* shaders, ElementSG* parent){
    m_scene_root = parent;
    m_item = nullptr;
    m_power = 1.f;

    auto* body_node = new NodeGameSG(shaders, parent, OBB_TYPE);
    m_camera = new NodeGameSG(shaders, body_node);

    Mesh* body_mesh = new Mesh(create_rectangle_cuboid({2,4,2}), true, OBB_TYPE);
    Mesh* arm_mesh = new Mesh("../assets/mesh/arm_2.obj", true, OBB_TYPE);

    body_node->get_trsf()->set_translation({2.,4,0});
    body_node->set_material(new MaterialColor(shaders, {0.75, 0., 0.95}, 50));
    body_node->set_debug_rendering(true);
    body_node->set_meshes({body_mesh});
    
    //Arm mesh 
    auto* arm_node = new NodeGameSG(shaders, m_camera, OBB_TYPE);
    arm_node->set_meshes({arm_mesh});
    arm_node->get_trsf()->set_rotation({0, -90, 0});
    arm_node->get_trsf()->set_translation({2, -2, -2});

    m_sight = CAMERA_INIT_FORWARD;

    //Initial camera (FPS)
    m_camera->get_trsf()->set_translation({0, 4, 0});
    // m_camera->get_trsf()->set_rotation({-10, 0, 0});

    m_body = new RigidBodyVolume(body_node, 10.f, 0.6f, 0.5f, true);

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


glm::vec3 Character::get_sight(){
    return m_sight;
}

void Character::set_sight(glm::vec3 sight){
    m_sight = sight;
}

void Character::grab_item(RigidBodyVolume* item, float action_area){
    
    glm::vec3 item_pos = item->get_node()->get_trsf()->get_translation();
    float distance = glm::length(item_pos - m_body->get_node()->get_trsf()->get_translation());
    
    if(distance < action_area){
        //ITEM can be grabbed
        m_item = item;
        m_item->clear_forces();

        //Item must be relative to camera
        m_item->get_node()->set_parent(m_camera);

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
    m_item->get_node()->get_trsf()->set_translation(4.f*m_sight);
}

void Character::throw_item(){
    glm::vec3 dir = m_camera->get_trsf()->apply_to_vector(m_sight);
    glm::vec3 throw_dir = m_power * dir;
    m_power = 0.f;
    RigidBodyVolume* item = m_item;
    m_item = nullptr;

    //save worldpos before getting new parent
    glm::vec3 pos = item->get_node()->get_position_in_world();
    
    item->get_node()->set_parent(m_scene_root);
    item->get_node()->get_trsf()->set_translation(pos);

    //Add force back and throw item.
    item->add_linear_impulse(throw_dir);
    item->add_force(new GravityForce());
    
}

void Character::jump(){
    float height_fact = 9.f;
    m_body->clear_forces();
    
    glm::vec3 vertical = height_fact*glm::vec3(0, 1, 0);
    m_body->set_linear_impulse(vertical);
    m_body->add_force(new GravityForce());

}

void Character::accumulate_power(){
    m_power += 0.2f;
}






