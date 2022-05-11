#include "ButtonElement.hpp"

ButtonElement::ButtonElement(
    ElementSG *parent, 
    BB_TYPE bb_type,
    DoorElement* door 
) : NodeGameSG(parent, bb_type){

    if(door != nullptr)
        m_linked.push_back(door);

    auto* red = new MaterialColor({1., 0.2, 0.2}, 500);

    auto *top_mesh = new Mesh("../assets/mesh/props/button_1.obj", true, OBB_TYPE);
    // auto *bottom_mesh = new Mesh("../assets/mesh/props/button_2.obj", true, OBB_TYPE);

    NodeGameSG* top_node = new NodeGameSG(this, SPHEREBB_TYPE);
    // NodeGameSG* bottom_node = new NodeGameSG(shaders, this, OBB_TYPE);

    top_node->get_trsf()->set_translation({0, 15,0});
    top_node->set_material(red);

    top_node->set_meshes({top_mesh});

    m_top = new RigidBodyVolume(top_node);

    // m_trsf->set_translation({-70, 12, 0});
    // m_trsf->set_uniform_scale(3);
}

void ButtonElement::add_behavior(float mult, std::vector<RigidBodyVolume*> rbv){
    auto* green = new MaterialColor({0.2, 1., 0.2}, 500);

    m_top->add_behavior(new MovementBehavior(true,false,mult,0,0.2,1));
    auto* color_behav = new SwitchColorBehavior(green);

    size_t size_rbv = rbv.size();
    for(int i = 0; i < size_rbv; i++){
        color_behav->can_collide_with(rbv[i]);
    }
    m_top->add_behavior(color_behav);
    if(!m_linked.empty()){
        DoorElement* door = m_linked[0];
        auto* move_door_behav =new MoveDoorBehavior(m_linked[0]);
        for(int i = 0; i < size_rbv; i++){
            move_door_behav->can_collide_with(rbv[i]);
        }
        m_top->add_behavior(move_door_behav);
    }
    // m_bottom->add_behavior(new MovementBehavior(false,false,mult,0,0.2,1));
}

void ButtonElement::link_door(DoorElement* door){
    m_linked.push_back(door);
}

RigidBodyVolume* ButtonElement::get_rigid_body(){
    return m_top;
}




