#include "RigidBodyVolume.hpp"
#include "src/physics/Collision.hpp"
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

void RigidBodyVolume::update(float delta_time) {
    float damping = 0.98f;
    glm::vec3 acceleration = m_forces * inverse_mass();

    m_velocity += acceleration * delta_time;
    m_velocity *= damping;

    glm::vec3 translation_modification_world = m_velocity * delta_time;

    ((NodeSG *) m_node_game)->get_trsf()->set_translation(
            ((NodeSG *) m_node_game)->get_trsf()->get_translation() + translation_modification_world);
    ((NodeSG *) m_node_game)->get_trsf()->compute();
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
        case SphereBB_TYPE:
            switch (bb2->get_type()) {
                case SphereBB_TYPE:
                    collision = ((SphereBB &) *bb1).get_data_collision((SphereBB &) *bb2);
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

void RigidBodyVolume::apply_impulse(RigidBodyVolume &rbv, const Collision &collision) {
    float inv_ma = inverse_mass();
    float inv_mb = rbv.inverse_mass();
    float inv_mass_sum = inv_ma + inv_mb;

    if (inv_mass_sum == 0.f)
        return;

    glm::vec3 rel_velocity = rbv.m_velocity - m_velocity;
    glm::vec3 rel_normal = glm::normalize(collision.normal);

    float dot_normal_velocity = glm::dot(rel_normal, rel_velocity);

    //objects move away from each other
    if (dot_normal_velocity > 0.f)
        return;

    float bounce = fminf(m_cor, rbv.m_cor);
    float numerator = (-(1.f + bounce) * dot_normal_velocity);
    float j = numerator / inv_mass_sum;
    
    // if (collision.contacts.size() >= 0 && j != 0.f) {
    //     j /= (float) collision.contacts.size();
    // }

    //linear impulse
    glm::vec3 impulse = rel_normal * j;
    m_velocity -= impulse *  inv_ma;
    rbv.m_velocity += impulse *  inv_mb;


    //friction
    glm::vec3 t = rel_velocity - (rel_normal * dot_normal_velocity);
    if (cmp_float(glm::dot(t, t), 0.f))
        return;
    t = glm::normalize(t);

    numerator = -glm::dot(rel_velocity, t);
    float jt = numerator / inv_mass_sum;
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
}

NodeGameSG *RigidBodyVolume::get_node() {
    return m_node_game;
}

float RigidBodyVolume::get_mass() const {
    return m_mass;
}

void RigidBodyVolume::set_forces(const glm::vec3 &forces) {
    m_forces = forces;
}

glm::vec3 RigidBodyVolume::get_forces() const {
    return m_forces;
}

void RigidBodyVolume::add_force(Force *f) {
    m_list_forces.push_back(f);
}

