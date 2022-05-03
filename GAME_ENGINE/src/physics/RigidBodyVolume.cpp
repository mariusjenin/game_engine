#include "RigidBodyVolume.hpp"
#include "src/physics/Collision.hpp"
#include "src/physics/ode/ODEFactory.hpp"
#include "src/utils/printer.hpp"
#include <glm/gtx/matrix_decompose.hpp>

using namespace physics;
using namespace physics::force;
using namespace scene_graph;

RigidBodyVolume::RigidBodyVolume(NodeGameSG *ng,float mass, float friction, float cor) {
    m_node_game = ng;
    m_node_game->set_rigid_body(this);
    m_mass = mass;
    m_friction = friction;
    m_cor = cor;
    m_list_forces = {};
}

RigidBodyVolume::~RigidBodyVolume() = default;

void RigidBodyVolume::update(float delta_time, ODE* ode) {

    ode->update(this,delta_time);
}

void RigidBodyVolume::apply_forces() {
    m_forces = {0,0,0};
    for(auto* force: m_list_forces){

        force->apply(this);
    }
}

Collision RigidBodyVolume::find_data_collision(RigidBodyVolume &rbv) {
    Collision collision;

    BoundingBox *bb1 = m_node_game->get_bb();
    BoundingBox *bb2 = rbv.m_node_game->get_bb();
    switch (bb1->get_type()) {
        case SPHEREBB_TYPE:
            switch (bb2->get_type()) {
                case SPHEREBB_TYPE:
                    collision = ((SphereBB &) *bb1).get_data_collision((SphereBB &) *bb2);
                    break;
                case OBB_TYPE:
                    collision = ((SphereBB &) *bb1).get_data_collision((OBB &) *bb2);
                break;
                default:
                    break;
            }
            break;
        case OBB_TYPE:
            switch (bb2->get_type()) {
                case OBB_TYPE:
                    collision = ((OBB &) *bb1).get_data_collision((OBB &) *bb2);
                    break;
                case SPHEREBB_TYPE:

                    collision = ((SphereBB &) *bb2).get_data_collision((OBB &) *bb1);
                    break;
                default:
                    break;
            }
            break;
        default:
            collision = Collision();
            break;
    }
    collision.rigid_body_1 = this;
    collision.rigid_body_2 = &rbv;
    return collision;
}

float RigidBodyVolume::inverse_mass() const {
    //Inverse only if mass != 0
    if (m_mass != 0.f)
        return 1.f / m_mass;

    return m_mass;
}

void RigidBodyVolume::add_linear_impulse(glm::vec3 &impulse) {
    m_velocity += impulse;
}

void RigidBodyVolume::apply_impulse(RigidBodyVolume &rbv, const Collision &collision, int index_contact) {
    float inv_ma = inverse_mass();
    float inv_mb = rbv.inverse_mass();
    float inv_mass_sum = inv_ma + inv_mb;

    if (inv_mass_sum == 0.f)
        return;

    glm::vec3 r1 = collision.contacts[index_contact] - m_node_game->get_bb()->get_position();
    glm::vec3 r2 = collision.contacts[index_contact] - rbv.get_node()->get_bb()->get_position();
    glm::mat4 i1 = inverse_tensor();
    glm::mat4 i2 = rbv.inverse_tensor();

    glm::vec3 rel_velocity =(rbv.m_velocity + glm::cross(rbv.m_angular_velocity, r2))
                       - (m_velocity + glm::cross(m_angular_velocity, r1));

    glm::vec3 rel_normal = glm::normalize(collision.normal);

    float dot_normal_velocity = glm::dot(rel_normal, rel_velocity);

    //objects move away from each other
    if (dot_normal_velocity > 0.f)
        return;

    float bounce = fminf(m_cor, rbv.m_cor);
    float numerator = (-(1.f + bounce) * dot_normal_velocity);

    glm::vec3 val1 = glm::vec3(i1*glm::vec4(glm::cross(r1, rel_normal),0));
    glm::vec3 val2 = glm::vec3(i2*glm::vec4(glm::cross(r2, rel_normal),0));
    glm::vec3 d2 = glm::cross(val1, r1);
    glm::vec3 d3 = glm::cross(val2, r2);
    float denominator = inv_mass_sum + glm::dot(rel_normal, d2 + d3);

    float j = (denominator == 0.0f) ? 0.0f :
              numerator / denominator;

     if (collision.contacts.size() >= 0 && j != 0.f) {
         j /= (float) collision.contacts.size();
     }

    //linear impulse
    glm::vec3 impulse = rel_normal * j;
    m_velocity -= impulse *  inv_ma;
    rbv.m_velocity += impulse *  inv_mb;
    m_angular_velocity -= glm::vec3( i1*glm::vec4(glm::cross(r1, impulse),0));
    rbv.m_angular_velocity += glm::vec3(i2*glm::vec4(glm::cross(r2, impulse),0));

    //friction
    glm::vec3 t = rel_velocity - (rel_normal * dot_normal_velocity);
    if (cmp_float(glm::dot(t, t), 0.f))
        return;
    t = glm::normalize(t);

    numerator = -glm::dot(rel_velocity, t);
    d2 = glm::cross(glm::vec3(i1 * glm::vec4(glm::cross(r1, t),0)),r1);
    d3 = glm::cross(glm::vec3(i2 * glm::vec4(glm::cross(r2, t),0)),r2);
    denominator = inv_mass_sum + glm::dot(t, d2 + d3);
    if (denominator == 0.0f) {
        return;
    }
    float jt = numerator / denominator;
    if (collision.contacts.size() >= 0 && jt != 0.f) {
        jt /= (float) collision.contacts.size();
    }
    if (cmp_float(jt, 0.f))
        return;

    float friction = sqrtf(m_friction * rbv.m_friction);
    if (jt > j * friction) {
        jt = j * friction;
    } else if (jt < -j * friction) {
        jt = -j * friction;
    }

    glm::vec3 tan_impulse = t * jt;
    m_velocity -= tan_impulse * inv_ma;
    rbv.m_velocity += tan_impulse * inv_mb;
    m_angular_velocity -= glm::vec3(i1*glm::vec4(glm::cross(r1, tan_impulse),0));
    rbv.m_angular_velocity += glm::vec3(i2*glm::vec4(glm::cross(r2, tan_impulse),0));
}

NodeGameSG *RigidBodyVolume::get_node() {
    return m_node_game;
}

float RigidBodyVolume::get_mass() const {
    return m_mass;
}

void RigidBodyVolume::set_mass(float mass){
    m_mass = mass;
}


void RigidBodyVolume::set_forces(const glm::vec3 &forces) {
    m_forces = forces;
}

glm::vec3 RigidBodyVolume::get_forces() const {
    return m_forces;
}

glm::vec3 RigidBodyVolume::get_torques() const {
    return m_torques;
}

void RigidBodyVolume::add_force(Force *f) {
    m_list_forces.push_back(f);
}

void RigidBodyVolume::clear_forces(){
    m_list_forces.clear();
}


glm::vec3 RigidBodyVolume::get_velocity() const {
    return m_velocity;
}

glm::vec3 RigidBodyVolume::get_acceleration() const {
    return m_acceleration;
}

void RigidBodyVolume::set_velocity(const glm::vec3 &velocity) {
    m_velocity = velocity;
}

void RigidBodyVolume::set_angular_velocity(const glm::vec3 &angular_velocity) {
    m_angular_velocity = angular_velocity;
}

void RigidBodyVolume::set_acceleration(const glm::vec3 &acceleration) {
    m_acceleration = acceleration;
}

glm::vec3 RigidBodyVolume::get_angular_velocity() const {
    return m_angular_velocity;
}

glm::vec3 RigidBodyVolume::get_angular_acceleration() const {
    return m_angular_acceleration;
}

glm::mat4 RigidBodyVolume::inverse_tensor() const {
    if(m_mass == 0) return glm::mat4(0.f);
    glm::vec3 tensor = m_node_game->get_bb()->get_tensor();
    return glm::inverse(glm::mat4(
            tensor[0] * m_mass,0,0,0,
            0,tensor[1]* m_mass,0,0,
            0,0,tensor[2]* m_mass,0,
            0,0,0,1.0f
    ));
}

void RigidBodyVolume::add_rotational_impulse(glm::vec3 &point, glm::vec3 &impulse) {
    glm::vec3 center_of_mass = m_node_game->get_bb()->get_position();
    glm::vec3 torque = glm::cross(point - center_of_mass, impulse);
    glm::vec3 angular_acceleration = glm::vec3( inverse_tensor()*glm::vec4(torque,0.f));
    m_angular_velocity = m_angular_velocity + angular_acceleration;
}

