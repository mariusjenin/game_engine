
#include <algorithm>
#include "PhysicsSystem.hpp"
#include "RigidBodyVolume.hpp"

using namespace physics;

PhysicsSystem::PhysicsSystem(float lpp, float ps, int ii) {
    m_linear_projection_percent = lpp;
    m_penetration_slack = ps;
    m_impulse_iteration = ii;
    m_rigid_bodies = {};
    m_collisions = {};
}

void PhysicsSystem::add_rigid_body(physics::RigidBodyVolume *rbv) {
    m_rigid_bodies.push_back(rbv);
}

void PhysicsSystem::remove_rigid_body(physics::RigidBodyVolume *rbv) {
    auto position = std::find(m_rigid_bodies.begin(), m_rigid_bodies.end(), rbv);
    if (position != m_rigid_bodies.end()) m_rigid_bodies.erase(position);
}

void PhysicsSystem::update(glm::vec3 pos_camera,float delta_time) {
    m_collisions.clear();
    size_t size_rigid_bodies = m_rigid_bodies.size();
    //Refresh BB
    for(auto & rigid_body : m_rigid_bodies){
        rigid_body->get_node()->refresh_bb(pos_camera);
    }
    //Compute Collision
    for(int i = 0 ; i < (int) size_rigid_bodies;i++){
        for(int j = i ; j < (int) size_rigid_bodies;j++){
            if(i==j)continue;
            RigidBodyVolume* rbv1 = m_rigid_bodies.at(i);
            RigidBodyVolume* rbv2 = m_rigid_bodies.at(j);
            Collision collision = rbv1->find_data_collision(*rbv2);
            if(collision.colliding){
                m_collisions.push_back(collision);
            }
        }
    }
//    for(int i = 0 ; i < m_collisions.size();i++){
//        std::cout << i<< "-\n"<< m_collisions[i].to_string() << std::endl;
//    }
    //Apply Forces
    for(int i = 0 ; i < (int) size_rigid_bodies;i++){
        m_rigid_bodies.at(i)->apply_forces();
    }
    //Apply Impulse
    for(int i = 0 ; i < m_impulse_iteration;i++){
        for(auto collision : m_collisions){
            size_t size_contacts = collision.contacts.size();
            for(int j = 0 ; j < size_contacts;j++){
                collision.rigid_body_1->apply_impulse(*collision.rigid_body_2,collision);
            }
        }
    }
    //Update Bodies
    for(auto & rigid_body : m_rigid_bodies){
        rigid_body->update(delta_time);
    }

    for(const auto& collision : m_collisions){
        RigidBodyVolume* rbv1 = collision.rigid_body_1;
        RigidBodyVolume* rbv2 = collision.rigid_body_2;
        float total_mass = rbv1->inverse_mass() + rbv2->inverse_mass();
        if (total_mass == 0.0f) {
            continue;
        }
        float depth = fmaxf(collision.depth - m_penetration_slack, 0.0f);
        float scalar = depth / total_mass;
        glm::vec3 correction = collision.normal * scalar * m_linear_projection_percent;
        Transform * trsf_rbv1 = rbv1->get_node()->get_trsf();
        Transform * trsf_rbv2 = rbv2->get_node()->get_trsf();
        trsf_rbv1->set_translation(trsf_rbv1->get_translation() - correction * rbv1->inverse_mass());
        trsf_rbv2->set_translation(trsf_rbv2->get_translation() + correction * rbv2->inverse_mass());
    }
}
