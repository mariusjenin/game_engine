//
// Created by mariusjenin on 02/05/2022.
//

#include "EulerODE.hpp"
#include <src/physics/RigidBodyVolume.hpp>
#include "src/utils/printer.hpp"

using namespace physics::ode;

void EulerODE::update(RigidBodyVolume *rbv,float delta_time, bool use_angular) {
    float damping = 0.99f;
    glm::vec3 forces = rbv->get_forces();
    glm::vec3 acceleration =  forces * rbv->inverse_mass();
    glm::vec3 velocity = rbv->get_velocity() + acceleration * delta_time;
    velocity *= damping;
    rbv->set_velocity(velocity);

    glm::vec3 angular_acceleration = glm::vec3(rbv->inverse_tensor()*glm::vec4(rbv->get_torques(),0));
    glm::vec3 angular_velocity = rbv->get_angular_velocity() + angular_acceleration * delta_time;
    if(use_angular)
        rbv->set_angular_velocity(angular_velocity);

    glm::vec3 translation = velocity * delta_time;
    glm::vec3 rotation = angular_velocity * delta_time;
    rotation = {glm::degrees(rotation.x),glm::degrees(rotation.y),glm::degrees(rotation.z)};
    Transform* trsf_node = rbv->get_node()->get_trsf();
    trsf_node->set_translation(trsf_node->get_translation() + translation);
    if(use_angular)
        trsf_node->set_rotation(trsf_node->get_rotation() + rotation);
    trsf_node->compute();
}
