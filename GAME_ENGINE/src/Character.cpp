#include "Character.hpp"

Character::Character(Shaders* shaders, ElementSG* parent){
    m_scene_root = parent;
    m_item = nullptr;
    m_character_node = new NodeGameSG(shaders, parent);
    m_camera = new NodeGameSG(shaders, m_character_node);
    m_sight = CAMERA_INIT_FORWARD;

    //Initial camera (FPS)
    m_camera->get_trsf()->set_translation({0, 8, 0});
    m_camera->compute_trsf_scene_graph();
    // m_camera->get_trsf()->set_rotation({-10, 0, 0});
}

NodeGameSG* Character::get_camera(){
    return m_camera;
}

NodeGameSG* Character::get_character_node(){
    return m_character_node;
}

void Character::set_camera(NodeGameSG* cam){
    m_camera = cam;
    m_character_node->add_child(cam);
}

glm::vec3 Character::get_sight(){
    return m_sight;
}

void Character::set_sight(glm::vec3 sight){
    m_sight = sight;
}

void Character::grab_item(RigidBodyVolume* item, float action_area){
    
    glm::vec3 item_pos = item->get_node()->get_trsf()->get_translation();
    float distance = glm::length(item_pos - m_character_node->get_trsf()->get_translation());
    
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

//Keep item in front of camera when moving mouse
void Character::update_item(){
    m_item->get_node()->get_trsf()->set_translation(4.f*m_sight);
}

void Character::throw_item(){
    glm::vec3 throw_dir = 10.f * m_sight;
    
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




