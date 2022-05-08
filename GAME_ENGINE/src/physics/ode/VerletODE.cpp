//
// Created by mariusjenin on 02/05/2022.
//

#include "VerletODE.hpp"
#include <src/physics/RigidBodyVolume.hpp>
#include "src/physics/rigid_body_behavior/MovementBehavior.hpp"

using namespace physics::ode;
using namespace physics::rigid_body_behavior;

void VerletODE::update(RigidBodyVolume *rbv, float delta_time) {
    MovementBehavior* mov_behav = rbv->get_movement_behavior();
    float damping = 0.99f;

    if(!mov_behav->is_translatable() && !mov_behav->is_rotatable()) return;
    Transform* trsf_node = rbv->get_node()->get_trsf();
    glm::vec3 avg_velocity;
    glm::vec3 avg_angular_velocity;

    if(mov_behav->is_translatable()) {
        avg_velocity = damping * (mov_behav->get_velocity() + mov_behav->get_acceleration() * delta_time / 2.0f);
        glm::vec3 translation = avg_velocity * delta_time;
        trsf_node->set_translation(trsf_node->get_translation() + translation);
    }
    if(mov_behav->is_rotatable()) {
        avg_angular_velocity =damping *
                (mov_behav->get_angular_velocity() + mov_behav->get_angular_acceleration() * delta_time / 2.0f);
        glm::vec3 rotation = avg_angular_velocity * delta_time;
        rotation = {glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z)};
        trsf_node->set_rotation(trsf_node->get_rotation() + rotation);
    }

    trsf_node->compute();


    // Calculate new acceleration and velocity
    if(mov_behav->is_translatable()) {
        glm::vec3 acceleration = damping * (mov_behav->get_forces() * mov_behav->inverse_mass());
        mov_behav->set_acceleration(acceleration);
        mov_behav->set_velocity(avg_velocity + acceleration * delta_time / 2.0f);
    }
    if(mov_behav->is_rotatable()) {
        glm::vec3 angular_acceleration = damping * glm::vec3(
                mov_behav->inverse_tensor() * glm::vec4(mov_behav->get_torques(), 0));
        glm::vec3 angular_velocity = mov_behav->get_angular_velocity() + angular_acceleration * delta_time / 2.f;
        mov_behav->set_angular_velocity(angular_velocity);
    }
}

VerletODE::VerletODE() {
    m_type = VERLET_TYPE;
}
