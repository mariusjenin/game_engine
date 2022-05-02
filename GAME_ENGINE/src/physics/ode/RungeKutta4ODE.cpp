//
// Created by mariusjenin on 02/05/2022.
//

#include "RungeKutta4ODE.hpp"
#include <src/physics/RigidBodyVolume.hpp>
#include <src/utils/printer.hpp>


using namespace physics::ode;

void RungeKutta4ODE::update(RigidBodyVolume *rbv, float delta_time) {
    glm::vec3 start_pos, start_vel, start_accel,start_ang_vel,start_ang_accel,start_rot;
    glm::vec3 pos, pos1, pos2, pos3, pos4,rot, rot1, rot2, rot3, rot4, vel, vel1, vel2, vel3, vel4,ang_vel, ang_vel1, ang_vel2, ang_vel3, ang_vel4;
    Transform* trsf_node = rbv->get_node()->get_trsf();
    start_pos = trsf_node->get_translation();
    start_rot = trsf_node->get_rotation();
    start_vel = rbv->get_velocity();
    start_accel = rbv->get_forces() * rbv->inverse_mass();
    start_ang_vel = rbv->get_angular_velocity();
    start_ang_accel = glm::vec3(rbv->inverse_tensor()*glm::vec4(rbv->get_torques(),0));

    pos1 = start_vel * delta_time;
    vel1 = start_accel * delta_time;
    rot1 = start_ang_vel * delta_time;
    ang_vel1 = start_ang_accel * delta_time;

    pos2 = (start_vel + vel1 / 2.f) * delta_time;
    vel2 = (start_accel + start_accel * pos1 / 2.f) * delta_time;
    rot2 = (start_ang_vel + ang_vel1 / 2.f) * delta_time;
    ang_vel2 = (start_ang_accel + start_ang_accel * pos1 / 2.f) * delta_time;

    pos3 = (start_vel + vel2 / 2.f) * delta_time;
    vel3 = (start_accel + start_accel * pos2 / 2.f) * delta_time;
    rot3 = (start_ang_vel + ang_vel2 / 2.f) * delta_time;
    ang_vel3 = (start_ang_accel + start_ang_accel * pos2 / 2.f) * delta_time;

    pos4 = (start_vel + vel3 / 2.f) * delta_time;
    vel4 = (start_accel + start_accel * pos3 / 2.f) * delta_time;
    rot4 = (start_ang_vel + ang_vel3 / 2.f) * delta_time;
    ang_vel4 = (start_ang_accel + start_ang_accel * pos3 / 2.f) * delta_time;

    pos = (pos1 + pos2 * 2.f + pos3 * 2.f + pos4) / 6.f; //new delta position
    vel = (vel1 + vel2 * 2.f + vel3 * 2.f + vel4) / 6.f; //new delta velocity
    rot = (rot1 + rot2 * 2.f + rot3 * 2.f + rot4) / 6.f; //new delta rotation
    ang_vel = (ang_vel1 + ang_vel2 * 2.f + ang_vel3 * 2.f + ang_vel4) / 6.f; //new delta angular velocity

    rbv->set_velocity(start_vel + vel);
    rbv->set_angular_velocity(start_ang_vel + ang_vel);
    trsf_node->set_translation(start_pos + pos);
    rot = {glm::degrees(rot.x),glm::degrees(rot.y),glm::degrees(rot.z)};
    trsf_node->set_rotation(start_rot + rot);
    trsf_node->compute();
}