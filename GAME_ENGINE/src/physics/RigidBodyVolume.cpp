#include "RigidBodyVolume.hpp"

using namespace physics;

RigidBodyVolume::RigidBodyVolume() = default;

RigidBodyVolume::~RigidBodyVolume() = default;

void RigidBodyVolume::update(float delta_time) {
    float damping = 0.98f;
    glm::vec3 acceleration = m_forces * inverse_mass();

    m_velocity += acceleration * delta_time;
    m_velocity *= damping;

    m_position += m_velocity * delta_time;
    synchro_collision_volumes();
}

void RigidBodyVolume::apply_forces() {
    m_forces = glm::vec3(0., -9.81f, 0.) * m_mass;
}

Collision RigidBodyVolume::find_data_collision(RigidBodyVolume &rbv) {
    switch (m_bb->get_type()) {
        case SphereBB_TYPE:
            switch (m_bb->get_type()) {
                case SphereBB_TYPE:
                    return PhysicsGeometry::get_data_collision((SphereBB &) *m_bb, (SphereBB &) *rbv.get_bb());
                default:
                    break;
            }
            break;
        default:
            break;
    }
    Collision res;
    res.reset();
    return res;
}

BoundingBox *RigidBodyVolume::get_bb() const {
    return m_bb;
}

void RigidBodyVolume::synchro_collision_volumes() {
    m_bb->set_position(m_position);
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

void RigidBodyVolume::apply_impulse(RigidBodyVolume &rbv, Collision c) {
    float inv_ma = inverse_mass();
    float inv_mb = rbv.inverse_mass();

    if (inv_ma + inv_mb == 0.f)
        return;

    glm::vec3 rel_velocity = rbv.m_velocity - m_velocity;
    glm::vec3 rel_normal = glm::normalize(c.normal);

    //Les objets s'éloignent
    if (glm::dot(rel_normal, rel_velocity) > 0.f)
        return;

    float bounce = fminf(m_cor, rbv.m_cor);
    float numerator = (-(1.f + bounce) * glm::dot(rel_normal, rel_velocity));
    float j = numerator / (inv_ma + inv_mb);
    if (c.contacts.size() >= 0 && j != 0.f) {
        j /= (float) c.contacts.size();
    }

    //impulsion linéaire
    glm::vec3 impulse = rel_normal * j;
    m_velocity -= impulse * inv_ma;
    rbv.m_velocity += impulse * inv_mb;


    //friction
    glm::vec3 t = rel_velocity - (rel_normal * glm::dot(rel_velocity, rel_normal));
    if (cmp_float(glm::dot(t, t), 0.f))
        return;

    t = glm::normalize(t);
    numerator = -glm::dot(rel_velocity, t);
    float jt = numerator / (inv_ma + inv_mb);
    if (c.contacts.size() >= 0 && jt != 0.f) {
        jt /= (float) c.contacts.size();
    }

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
