//
// Created by mariusjenin on 07/05/2022.
//

#include "SwitchColorBehavior.hpp"


using namespace physics::rigid_body_behavior;


SwitchColorBehavior::SwitchColorBehavior(Material* material) {
    m_type = SwitchColor_TYPE;
    m_material = material;
    m_timer = 0;
}

void SwitchColorBehavior::action(PhysicsSystem* ps,Collision collision,float delta_time) {
    bool action = false;
    size_t size_rbv = m_rigid_bodies.size();
    for(int i = 0; i < size_rbv; i++){
        if(m_rigid_bodies[i] == collision.rigid_body_1 || m_rigid_bodies[i] == collision.rigid_body_2){
            action=true;
            break;
        }
    }
    if(action){
        if(m_timer <=0){
            switch_color();
        }
        m_timer = 2;
    }
}

void SwitchColorBehavior::update_physics(float delta_time) {
    //Nothing
}

void SwitchColorBehavior::update_render(float delta_time, ODE* ode) {
    if(m_timer>0){
        m_timer -= delta_time;
        if(m_timer<=0){
            switch_color();
        }
    }
}

void SwitchColorBehavior::switch_color(){
    Material* tmp_material = m_rigid_body->get_node()->get_material();
    m_rigid_body->get_node()->set_material(m_material);
    m_material = tmp_material;
}

void SwitchColorBehavior::can_collide_with(RigidBodyVolume *rbv) {
    m_rigid_bodies.push_back(rbv);
}
