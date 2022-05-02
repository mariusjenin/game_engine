//
// Created by mariusjenin on 02/05/2022.
//

#include "RungeKutta4ODE.hpp"
#include <src/physics/RigidBodyVolume.hpp>
#include <src/utils/printer.hpp>


using namespace physics::ode;

void RungeKutta4ODE::update(RigidBodyVolume *rbv, float delta_time) {
    glm::vec3 start_pos, start_vel, start_accel;
    glm::vec3 pos, pos1, pos2, pos3, pos4, vel, vel1, vel2, vel3, vel4;
    Transform* trsf_node = rbv->get_node()->get_trsf();
    start_pos = trsf_node->get_translation();
    start_vel = rbv->get_velocity();
    start_accel = rbv->get_forces() * rbv->inverse_mass();

    pos1 = start_vel * delta_time;
    vel1 = start_accel * delta_time;

    pos2 = (start_vel + vel1 / 2.f) * delta_time;
    vel2 = (start_accel + start_accel * pos1 / 2.f) * delta_time;

    pos3 = (start_vel + vel2 / 2.f) * delta_time;
    vel3 = (start_accel + start_accel * pos2 / 2.f) * delta_time;

    pos4 = (start_vel + vel3 / 2.f) * delta_time;
    vel4 = (start_accel + start_accel * pos3 / 2.f) * delta_time;

    pos = (pos1 + pos2 * 2.f + pos3 * 2.f + pos4) / 6.f; //new delta position
    vel = (vel1 + vel2 * 2.f + vel3 * 2.f + vel4) / 6.f; //new delta velocity

    rbv->set_velocity(start_vel + vel);
    trsf_node->set_translation(start_pos + pos);
    trsf_node->compute();
}