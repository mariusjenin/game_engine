
#include <algorithm>
#include "PhysicsSystem.hpp"
#include "RigidBodyVolume.hpp"
#include "src/physics/ode/ODEFactory.hpp"

using namespace physics;

PhysicsSystem::PhysicsSystem(ElementSG* root_physics,float lpp, float ps, int ii, ODE_TYPE ode_type) {
    m_linear_projection_percent = lpp;
    m_penetration_slack = ps;
    m_impulse_iteration = ii;
    m_rigid_bodies = {};
    m_collisions = {};
    m_ode = ODEFactory::generate_ode(ode_type);
    m_root_physics = root_physics;
}

void PhysicsSystem::add_rigid_body(physics::RigidBodyVolume *rbv) {
    m_rigid_bodies.push_back(rbv);
}

void PhysicsSystem::remove_rigid_body(physics::RigidBodyVolume *rbv) {
    auto position = std::find(m_rigid_bodies.begin(), m_rigid_bodies.end(), rbv);
    if (position != m_rigid_bodies.end()) m_rigid_bodies.erase(position);
}

void PhysicsSystem::remove_rigid_body_with_node(NodeGameSG* node) {
    size_t size_rbv = m_rigid_bodies.size();
    for(int i = 0; i < size_rbv;i++){
        if(m_rigid_bodies[i]->get_node() == node){
            m_rigid_bodies.erase(m_rigid_bodies.begin()+i);
            break;
        }
    }
}

void PhysicsSystem::clear_rigid_bodies() {
    size_t rbv_size = m_rigid_bodies.size();
    for(int i = 0; i < rbv_size;i++){
        delete m_rigid_bodies[i];
    }
    m_rigid_bodies.clear();
}

void PhysicsSystem::update_bodies(glm::vec3 pos_camera,float delta_time) {
    refresh_bodies_bb(pos_camera);

    for(auto & rigid_body : m_rigid_bodies){
        rigid_body->update(delta_time,m_ode);
    }

}

void PhysicsSystem::refresh_bodies_bb(glm::vec3 pos_camera){
    //Refresh BB
    for(auto & rigid_body : m_rigid_bodies){
        rigid_body->get_node()->refresh_bb(pos_camera);
    }
    
    m_root_physics->reset_trsf_dirty(false);
    m_root_physics->reset_children_dirty(false);
}

void PhysicsSystem::update_collisions(glm::vec3 pos_camera,float delta_time) {
    m_collisions.clear();
    size_t size_rigid_bodies = m_rigid_bodies.size();
    //Refresh BB
    refresh_bodies_bb(pos_camera);
    //Compute Collision //TODO with BoundingBoxes
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

    //Apply Forces
    for(int i = 0 ; i < (int) size_rigid_bodies;i++){
        m_rigid_bodies.at(i)->apply_forces();
    }
    //Apply Impulse
    for(int i = 0 ; i < m_impulse_iteration;i++){
        for(auto collision : m_collisions){
            size_t size_contacts = collision.contacts.size();
            for(int j = 0 ; j < size_contacts;j++){
                collision.rigid_body_1->apply_impulse(*collision.rigid_body_2,collision,j);
            }
        }
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

        //JOINT CONSTRAINT
        BoundingBox* bb1 = rbv1->get_node()->get_bb();
        BoundingBox* bb2 = rbv2->get_node()->get_bb();

        glm::vec3 p1 = bb1->closest_point(bb2->get_position());
        glm::vec3 p2 = bb2->closest_point(bb1->get_position());

        //Adds stability
        if(glm::length(p2 - p1) > 0.1f){
            trsf_rbv1->set_translation(trsf_rbv1->get_translation() - correction * rbv1->inverse_mass());
            trsf_rbv2->set_translation(trsf_rbv2->get_translation() + correction * rbv2->inverse_mass());
            
        }
    }
}
