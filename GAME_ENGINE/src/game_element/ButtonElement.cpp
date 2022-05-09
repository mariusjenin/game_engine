#include "ButtonElement.hpp"

ButtonElement::ButtonElement(
    Shaders *shaders, 
    ElementSG *parent, 
    BB_TYPE bb_type,
    DoorElement* door 
) : NodeGameSG(shaders, parent, bb_type){

    if(door != nullptr)
        m_linked.push_back(door);

    auto* lab_mat_color = new MaterialColor(m_shaders, {1., 0.8, 0.7}, 50);

    auto *top_mesh = new Mesh("../assets/mesh/props/button_1.obj", true, OBB_TYPE);
    // auto *bottom_mesh = new Mesh("../assets/mesh/props/button_2.obj", true, OBB_TYPE);

    NodeGameSG* top_node = new NodeGameSG(shaders, this, SPHEREBB_TYPE);
    // NodeGameSG* bottom_node = new NodeGameSG(shaders, this, OBB_TYPE);

    top_node->get_trsf()->set_translation({0, 15,0});
    top_node->set_material(lab_mat_color);

    top_node->set_meshes({top_mesh});

    m_top = new RigidBodyVolume(top_node);

    // m_trsf->set_translation({-70, 12, 0});
    // m_trsf->set_uniform_scale(3);
    // set_debug_rendering(true);
}

void ButtonElement::add_behavior(float mult){
    m_top->add_behavior(new MovementBehavior(true,false,mult,0,0.2,1));
    if(m_linked.size() > 0){
        DoorElement* door = m_linked[0];
        m_top->add_behavior(new MoveDoorBehavior(m_linked[0]));
    }
    // m_bottom->add_behavior(new MovementBehavior(false,false,mult,0,0.2,1));
}

void ButtonElement::link_door(DoorElement* door){
    m_linked.push_back(door);
}

RigidBodyVolume* ButtonElement::get_top(){
    return m_top;
}




