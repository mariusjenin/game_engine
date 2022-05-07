//
// Created by mariusjenin on 27/04/2022.
//

#include "GravityForce.hpp"
#include "src/physics/rigid_body_behavior/MovementBehavior.hpp"

using namespace physics::rigid_body_behavior;

void GravityForce::apply(RigidBodyVolume* rbv) {
    MovementBehavior* mov_behav = rbv->get_movement_behavior();
    mov_behav->set_forces(mov_behav->get_forces()+mov_behav->get_mass()*m_direction*m_gravity);
}

GravityForce::GravityForce(glm::vec3 dir,float g) {
    m_direction = dir;
    m_gravity = g;
}
