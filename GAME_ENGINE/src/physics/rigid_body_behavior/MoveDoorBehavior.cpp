#include "MoveDoorBehavior.hpp"


using namespace physics::rigid_body_behavior;


MoveDoorBehavior::MoveDoorBehavior(DoorElement* door) {
    m_type = MoveDoor_TYPE;
    m_door = door;
    m_timer = 0;


}

void MoveDoorBehavior::action(PhysicsSystem* ps,Collision collision,float delta_time) {
    if(m_timer <=0){
        m_door->open();
    }
    m_timer = 4;
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
