
#include <algorithm>
#include "PhysicsSystem.hpp"
#include "RigidBodyVolume.hpp"
#include "src/physics/ode/ODEFactory.hpp"

using namespace physics;

PhysicsSystem::PhysicsSystem(ElementSG* root_physics,float mult_physics, float lpp, float ps, int ii, ODE_TYPE ode_type) {
    m_linear_projection_percent = lpp;
    m_penetration_slack = ps;
    m_impulse_iteration = ii;
    m_colliders = {};
    m_ode = ODEFactory::generate_ode(ode_type);
    m_root_physics = root_physics;
    m_multiplicator = mult_physics;
}

void PhysicsSystem::add_collider(physics::RigidBodyVolume *rbv) {
    m_colliders.push_back(rbv);
}

void PhysicsSystem::remove_collider(physics::RigidBodyVolume *rbv) {
    auto position = std::find(m_colliders.begin(), m_colliders.end(), rbv);
    if (position != m_colliders.end()) m_colliders.erase(position);
}

void PhysicsSystem::remove_collider_with_node(NodeGameSG* node) {
    size_t size_rbv = m_colliders.size();
    for(int i = 0; i < size_rbv;i++){
        if(m_colliders[i]->get_node() == node){
            m_colliders.erase(m_colliders.begin() + i);
            break;
        }
    }
}

void PhysicsSystem::clear_rigid_bodies() {
    size_t rbv_size = m_colliders.size();
    for(int i = 0; i < rbv_size;i++){
        delete m_colliders[i];
    }
    m_colliders.clear();
}

void PhysicsSystem::update_bodies(glm::vec3 pos_camera,float delta_time) {
    refresh_bodies_bb(pos_camera);
    for(auto & rigid_body : m_colliders){
        rigid_body->update_render(delta_time,m_ode);
    }
}

void PhysicsSystem::refresh_bodies_bb(glm::vec3 pos_camera){
    //Refresh BB
    for(auto & rigid_body : m_colliders){
        rigid_body->get_node()->refresh_bb(pos_camera);
    }

    m_root_physics->reset_trsf_dirty(false);
    m_root_physics->reset_children_dirty(false);
}

void PhysicsSystem::update_collisions(glm::vec3 pos_camera,float delta_time) {
    //Refresh BB
    refresh_bodies_bb(pos_camera);
    //Compute Collision
    size_t size_rigid_bodies = m_colliders.size();
    for (int i = 0; i < (int) size_rigid_bodies; i++) {
        for (int j = i; j < (int) size_rigid_bodies; j++) {
            if (i == j)continue;
            RigidBodyVolume *rbv1 = m_colliders.at(i);
            RigidBodyVolume *rbv2 = m_colliders.at(j);
            Collision collision = rbv1->find_data_collision(*rbv2);

            if (collision.colliding) {
                //Do an action if colliding
                collision.rigid_body_1->action(this, collision, delta_time);
                collision.rigid_body_2->action(this, collision, delta_time);
            }
        }
    }

    for (int i = 0; i < (int) size_rigid_bodies; i++) {
        //Update at each physic refresh
        m_colliders.at(i)->update_physics(delta_time);
    }
}

int PhysicsSystem::get_impulse_iteration() const {
    return m_impulse_iteration;
}

float PhysicsSystem::get_linear_projection_percent() const {
    return m_linear_projection_percent;
}

float PhysicsSystem::get_penetration_slack() const {
    return m_penetration_slack;
}

void PhysicsSystem::set_multiplicator_physics(float mult_physics) {
    m_multiplicator = mult_physics;
}

float PhysicsSystem::get_multiplicator_physics() const{
    return m_multiplicator;
}
