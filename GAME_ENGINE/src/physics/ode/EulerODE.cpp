//
// Created by mariusjenin on 02/05/2022.
//

#include "EulerODE.hpp"
#include <src/physics/RigidBodyVolume.hpp>
#include "src/utils/printer.hpp"
#include "src/physics/rigid_body_behavior/MovementBehavior.hpp"

using namespace physics::ode;
using namespace physics::rigid_body_behavior;

void EulerODE::update(RigidBodyVolume *rbv, float delta_time) {
    MovementBehavior* mov_behav = rbv->get_movement_behavior();
    if(!mov_behav->is_translatable() && !mov_behav->is_rotatable()) return;
    Transform* trsf_node = rbv->get_node()->get_trsf();
    float damping = 0.99f;

    if(mov_behav->is_translatable()){
        glm::vec3 acceleration =  mov_behav->get_forces() * mov_behav->inverse_mass();
        glm::vec3 velocity = mov_behav->get_velocity() + acceleration * delta_time;
        velocity *= damping;
        mov_behav->set_velocity(velocity);
        glm::vec3 translation = velocity * delta_time;
        trsf_node->set_translation(trsf_node->get_translation() + translation);
    }
    if(mov_behav->is_rotatable()){
        glm::vec3 angular_acceleration = glm::vec3(mov_behav->inverse_tensor()*glm::vec4(mov_behav->get_torques(),0));
        glm::vec3 angular_velocity = mov_behav->get_angular_velocity() + angular_acceleration * delta_time;
        angular_velocity *= damping;
        mov_behav->set_angular_velocity(angular_velocity);
        glm::vec3 rotation = angular_velocity * delta_time;
        rotation = {glm::degrees(rotation.x),glm::degrees(rotation.y),glm::degrees(rotation.z)};
        trsf_node->set_rotation(trsf_node->get_rotation() + rotation);
    }
    trsf_node->compute();
}

EulerODE::EulerODE() {
    m_type = EULER_TYPE;
}