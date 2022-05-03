//
// Created by mariusjenin on 02/05/2022.
//

#include "VerletODE.hpp"
#include <src/physics/RigidBodyVolume.hpp>

using namespace physics::ode;

void VerletODE::update(RigidBodyVolume *rbv,float delta_time, bool use_angular) {
    // Velocity Verlet Integration
    glm::vec3 avg_velocity = rbv->get_velocity() + rbv->get_acceleration() * delta_time / 2.0f;
    glm::vec3 avg_angular_velocity = rbv->get_angular_velocity() + rbv->get_angular_acceleration() * delta_time / 2.0f;

    // Position is integrated with the average velocity
    glm::vec3 translation = avg_velocity * delta_time;
    glm::vec3 rotation = avg_angular_velocity * delta_time;
    rotation = {glm::degrees(rotation.x),glm::degrees(rotation.y),glm::degrees(rotation.z)};
    Transform* trsf_node = rbv->get_node()->get_trsf();
    trsf_node->set_translation(trsf_node->get_translation() + translation);
    if(use_angular)
        trsf_node->set_rotation(trsf_node->get_rotation() + rotation);
    trsf_node->compute();

    // Calculate new acceleration and velocity
    glm::vec3 acceleration = rbv->get_forces()*rbv->inverse_mass();
    rbv->set_acceleration(acceleration);
    rbv->set_velocity(avg_velocity + acceleration * delta_time / 2.0f);
    glm::vec3 angular_acceleration = glm::vec3(rbv->inverse_tensor()*glm::vec4(rbv->get_torques(),0));
    glm::vec3 angular_velocity = rbv->get_angular_velocity() + angular_acceleration * delta_time / 2.f;
    rbv->set_angular_velocity(angular_velocity);
}
