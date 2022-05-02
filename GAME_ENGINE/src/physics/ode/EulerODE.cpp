//
// Created by mariusjenin on 02/05/2022.
//

#include "EulerODE.hpp"
#include <src/physics/RigidBodyVolume.hpp>

using namespace physics::ode;

void EulerODE::update(RigidBodyVolume *rbv,float delta_time) {
    glm::vec3 forces = rbv->get_forces();
    glm::vec3 acceleration =  forces * rbv->inverse_mass();
    glm::vec3 velocity = rbv->get_velocity() + acceleration * delta_time;
    rbv->set_velocity(velocity);

    glm::vec3 translation = velocity * delta_time;
    Transform* trsf_node = rbv->get_node()->get_trsf();
    trsf_node->set_translation(trsf_node->get_translation() + translation);
    trsf_node->compute();
}
