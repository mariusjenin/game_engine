#include "DoorElement.hpp"

DoorElement::DoorElement(
    ElementSG *parent, 
    BB_TYPE bb_type,
    int id_left_tex,
    int id_right_tex
) : NodeGameSG(parent, bb_type){

    auto* lab_mat_color = new MaterialColor({1., 0.8, 0.7}, 50);

    auto *left_mesh = new Mesh("../assets/mesh/env/door_left_lab.obj", true, OBB_TYPE);
    auto *right_mesh = new Mesh("../assets/mesh/env/door_right_lab.obj", true, OBB_TYPE);
    // int id_blue_texture = texture_manager->load_texture("../assets/texture/red_texture.bmp");
    // int id_red_texture = texture_manager->load_texture("../assets/texture/blue_texture.bmp");

    NodeGameSG* left_node = new NodeGameSG(this, OBB_TYPE);
    NodeGameSG* right_node = new NodeGameSG(this, OBB_TYPE);

    right_node->get_trsf()->set_translation({0, 0, 1.15});
    left_node->set_material(new MaterialTexture(id_left_tex));
    right_node->set_material(new MaterialTexture(id_right_tex));

    right_node->set_meshes({right_mesh});
    left_node->set_meshes({left_mesh});

    m_left = new RigidBodyVolume(left_node);
    m_right = new RigidBodyVolume(right_node);

    m_trsf->set_translation({-70, 12, 0});
    m_trsf->set_uniform_scale(3);

    m_closed_pos.push_back(left_node->get_trsf()->get_translation());
    m_closed_pos.push_back(right_node->get_trsf()->get_translation());

    //modify opening space
    m_open_pos.push_back(m_closed_pos[0] + glm::vec3(0, 0, 3)); 
    m_open_pos.push_back(m_closed_pos[1] + glm::vec3(0, 0, -3));

    // set_debug_rendering(true);
}

void DoorElement::open(){
    glm::vec3 left_translate = m_left->get_node()->get_trsf()->get_translation();
    glm::vec3 right_translate = m_right->get_node()->get_trsf()->get_translation();
    
    if(left_translate[2] <= m_open_pos[0][2] || right_translate[2] >= m_open_pos[1][2]){
        m_left->get_node()->get_trsf()->set_translation(left_translate + glm::vec3(0, 0, 0.2));
        m_right->get_node()->get_trsf()->set_translation(right_translate + glm::vec3(0, 0, -0.2));
    }
}

void DoorElement::close(){
    glm::vec3 left_translate = m_left->get_node()->get_trsf()->get_translation();
    glm::vec3 right_translate = m_right->get_node()->get_trsf()->get_translation();

    if(left_translate[2] > m_closed_pos[0][2] || right_translate[2] < m_closed_pos[1][2]){
        m_left->get_node()->get_trsf()->set_translation(left_translate - glm::vec3(0, 0, 0.2));
        m_right->get_node()->get_trsf()->set_translation(right_translate - glm::vec3(0, 0, -0.2));
    
    }else{
        m_left->get_node()->get_trsf()->set_translation(glm::vec3(0, 0, 0));
        m_right->get_node()->get_trsf()->set_translation(glm::vec3(0, 0, 1.15));         
    }
}


void DoorElement::add_behavior(float mult){
    m_left->add_behavior(new MovementBehavior(true,false,mult,0,0.2,1));
    m_right->add_behavior(new MovementBehavior(true,false,mult,0,0.2,1));
}

RigidBodyVolume* DoorElement::get_left(){
    return m_left;
}

RigidBodyVolume* DoorElement::get_right(){
    return m_right;
}



