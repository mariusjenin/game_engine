//
// Created by mariusjenin on 27/04/2022.
//

#include "GravityForce.hpp"


void GravityForce::apply(RigidBodyVolume* rbv) {
    rbv->set_forces(rbv->get_forces()+rbv->get_mass()*m_direction*m_gravity);
}

GravityForce::GravityForce(glm::vec3 dir,float g) {
    m_direction = dir;
    m_gravity = g;
}
