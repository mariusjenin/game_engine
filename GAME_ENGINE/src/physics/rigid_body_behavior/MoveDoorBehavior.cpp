#include "MoveDoorBehavior.hpp"


using namespace physics::rigid_body_behavior;


MoveDoorBehavior::MoveDoorBehavior(DoorElement* door) {
    m_type = MoveDoor_TYPE;
    m_door = door;
    m_timer = 0;


}

void MoveDoorBehavior::action(PhysicsSystem* ps,Collision collision,float delta_time) {
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
            m_door->open();
        }
        m_timer = 4;
    }
}

void MoveDoorBehavior::update_physics(float delta_time) {
    //Nothing
}

void MoveDoorBehavior::update_render(float delta_time, ODE* ode) {
    if(m_timer>0){
        if(m_timer > 2){
            m_door->open();
        }else{

            m_door->close();
        }
        m_timer -= delta_time;

    }
}

void MoveDoorBehavior::can_collide_with(RigidBodyVolume *rbv) {
    m_rigid_bodies.push_back(rbv);
}