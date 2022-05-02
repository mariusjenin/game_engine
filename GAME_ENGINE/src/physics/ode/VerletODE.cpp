//
// Created by mariusjenin on 02/05/2022.
//

#include "VerletODE.hpp"
#include <src/physics/RigidBodyVolume.hpp>

using namespace physics::ode;

void VerletODE::update(RigidBodyVolume *rbv,float delta_time) {
    // Velocity Verlet Integration
    glm::vec3 avg_velocity = rbv->get_velocity() + rbv->get_acceleration() * delta_time / 2.0f;

    // Position is integrated with the average velocity
    glm::vec3 translation = avg_velocity * delta_time;
    Transform* trsf_node = rbv->get_node()->get_trsf();
    trsf_node->set_translation(trsf_node->get_translation() + translation);
    trsf_node->compute();

    // Calculate new acceleration and velocity
    glm::vec3 acceleration = rbv->get_forces()*rbv->inverse_mass();
    rbv->set_acceleration(acceleration);
    rbv->set_velocity(avg_velocity + acceleration * delta_time / 2.0f);
}
