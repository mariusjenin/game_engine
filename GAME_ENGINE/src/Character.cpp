#include "Character.hpp"

Character::Character(Shaders* shaders, ElementSG* parent){
    m_character_node = new NodeGameSG(shaders, parent);
    m_camera = new NodeGameSG(shaders, m_character_node);
    m_sight = CAMERA_INIT_FORWARD;

    //Initial camera (FPS)
    m_camera->get_trsf()->set_translation({0, 8, 0});
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

void Character::grab_item(NodeGameSG item){

}



