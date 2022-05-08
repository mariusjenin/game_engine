//
// Created by mariusjenin on 07/05/2022.
//

#include "MovementBehavior.hpp"

using namespace physics::rigid_body_behavior;


MovementBehavior::MovementBehavior(bool translatable,bool rotatable,float mult_physics,float mass, float friction, float cor) {
    m_type = MovementBehavior_TYPE;
    m_translatable = translatable;
    m_rotatable = rotatable;
    m_mass = mass;
    m_friction = friction;
    m_cor = cor;
    m_list_forces = {};
    m_multiplicator = mult_physics;
}

void MovementBehavior::action(PhysicsSystem *ps, Collision collision, float delta_time) {
    for (int k = 0; k < ps->get_impulse_iteration(); k++) {
        size_t size_contacts = collision.contacts.size();
        for (int l = 0; l < size_contacts; l++) {
            apply_impulse(*collision.rigid_body_2, collision, l);
        }
    }
    MovementBehavior *mov_behav = collision.rigid_body_2->get_movement_behavior();
    float total_mass = inverse_mass() + mov_behav->inverse_mass();
    if (total_mass == 0.0f) {
        return;
    }
    float depth = fmaxf(collision.depth - ps->get_penetration_slack(), 0.0f);
    float scalar = depth / total_mass;
    glm::vec3 correction = collision.normal * scalar * ps->get_linear_projection_percent();
    NodeGameSG *node1 = m_rigid_body->get_node();
    NodeGameSG *node2 = collision.rigid_body_2->get_node();
    Transform *trsf_rbv1 = node1->get_trsf();
    Transform *trsf_rbv2 = node2->get_trsf();

    //JOINT CONSTRAINT
    BoundingBox *bb1 = node1->get_bb();
    BoundingBox *bb2 = node2->get_bb();

    glm::vec3 p1 = bb1->closest_point(bb2->get_position());
    glm::vec3 p2 = bb2->closest_point(bb1->get_position());

    //Adds stability
    if (glm::length(p2 - p1) > 0.1f) {
        trsf_rbv1->set_translation(trsf_rbv1->get_translation() - correction * inverse_mass());
        trsf_rbv2->set_translation(trsf_rbv2->get_translation() + correction * mov_behav->inverse_mass());
    }
}

void MovementBehavior::update_physics(float delta_time) {
    m_forces = {0, 0, 0};
    for (auto *force: m_list_forces) {
        force->apply(m_rigid_body);
    }
    m_forces *= m_multiplicator;
}

void MovementBehavior::update_render(float delta_time, ODE *ode) {
    ode->update(m_rigid_body, delta_time);
}

void MovementBehavior::add_force(Force *f) {
    m_list_forces.push_back(f);
}

float MovementBehavior::inverse_mass() const {
    //Inverse only if mass != 0
    if (m_mass != 0.f)
        return 1.f / m_mass;

    return m_mass;
}

glm::mat4 MovementBehavior::inverse_tensor() const {
    if (m_mass == 0) return glm::mat4(0.f);
    BoundingBox *bb = m_rigid_body->get_node()->get_bb();
    if (bb == nullptr)return glm::mat4(0.f);
    glm::vec3 tensor = bb->get_tensor();
    return glm::inverse(glm::mat4(
            tensor[0] * m_mass, 0, 0, 0,
            0, tensor[1] * m_mass, 0, 0,
            0, 0, tensor[2] * m_mass, 0,
            0, 0, 0, 1.0f
    ));
}

void MovementBehavior::add_linear_impulse(glm::vec3 &impulse) {
    if(m_translatable){
        m_velocity += impulse;
    }
}

void MovementBehavior::add_rotational_impulse(glm::vec3 &point, glm::vec3 &impulse) {
    if(m_rotatable){
        BoundingBox *bb = m_rigid_body->get_node()->get_bb();
        if (bb == nullptr) return;
        glm::vec3 center_of_mass = bb->get_position();
        glm::vec3 torque = glm::cross(point - center_of_mass, impulse);
        glm::vec3 angular_acceleration = glm::vec3(inverse_tensor() * glm::vec4(torque, 0.f));
        m_angular_velocity = m_angular_velocity + angular_acceleration ;
    }
}

void MovementBehavior::apply_impulse(RigidBodyVolume &rbv, const Collision &collision, int index_contact) {
    MovementBehavior *mov_behav = rbv.get_movement_behavior();
    if(!m_translatable && !m_rotatable && !mov_behav->m_translatable && !mov_behav->m_rotatable) return;

    float inv_ma = inverse_mass();
    float inv_mb = mov_behav->inverse_mass();
    float inv_mass_sum = inv_ma + inv_mb;

    if (inv_mass_sum == 0.f)
        return;

    glm::vec3 r1 = collision.contacts[index_contact] - m_rigid_body->get_node()->get_bb()->get_position();
    glm::vec3 r2 = collision.contacts[index_contact] - rbv.get_node()->get_bb()->get_position();
    glm::mat4 i1 = inverse_tensor();
    glm::mat4 i2 = mov_behav->inverse_tensor();

    glm::vec3 rel_velocity = rbv.get_movement_behavior()->get_velocity() - m_velocity;


    rel_velocity = (mov_behav->m_velocity + glm::cross(mov_behav->m_angular_velocity, r2))
                   - (m_velocity + glm::cross(m_angular_velocity, r1));

    glm::vec3 rel_normal = glm::normalize(collision.normal);

    float dot_normal_velocity = glm::dot(rel_normal, rel_velocity);

    //objects move away from each other
    if (dot_normal_velocity > 0.f)
        return;

    float bounce = fminf(m_cor, mov_behav->m_cor);
    float numerator = (-(1.f + bounce) * dot_normal_velocity);

    glm::vec3 val1 = glm::vec3(i1 * glm::vec4(glm::cross(r1, rel_normal), 0));
    glm::vec3 val2 = glm::vec3(i2 * glm::vec4(glm::cross(r2, rel_normal), 0));
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
    if(m_translatable) m_velocity -= impulse * inv_ma;
    if(mov_behav->m_translatable) mov_behav->m_velocity += impulse * inv_mb;
    if(m_rotatable) m_angular_velocity -= glm::vec3(i1 * glm::vec4(glm::cross(r1, impulse), 0));
    if(mov_behav->m_rotatable) mov_behav->m_angular_velocity += glm::vec3(i2 * glm::vec4(glm::cross(r2, impulse), 0));


    //friction
    glm::vec3 t = rel_velocity - (rel_normal * dot_normal_velocity);
    if (cmp_float(glm::dot(t, t), 0.f))
        return;
    t = glm::normalize(t);

    numerator = -glm::dot(rel_velocity, t);
    d2 = glm::cross(glm::vec3(i1 * glm::vec4(glm::cross(r1, t), 0)), r1);
    d3 = glm::cross(glm::vec3(i2 * glm::vec4(glm::cross(r2, t), 0)), r2);
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

    float friction = sqrtf(m_friction * mov_behav->m_friction);
    if (jt > j * friction) {
        jt = j * friction;
    } else if (jt < -j * friction) {
        jt = -j * friction;
    }

    glm::vec3 tan_impulse = t * jt;
    if(m_translatable)  m_velocity -= tan_impulse * inv_ma;
    if(mov_behav->m_translatable) mov_behav->m_velocity += tan_impulse * inv_mb;
    if(m_rotatable) m_angular_velocity -= glm::vec3(i1 * glm::vec4(glm::cross(r1, tan_impulse), 0));
    if(mov_behav->m_rotatable) mov_behav->m_angular_velocity += glm::vec3(i2 * glm::vec4(glm::cross(r2, tan_impulse), 0));
}

float MovementBehavior::get_mass() const {
    return m_mass;
}

void MovementBehavior::set_mass(float mass) {
    m_mass = mass;
}

void MovementBehavior::set_forces(const glm::vec3 &forces) {
    m_forces = forces;
}

void MovementBehavior::clear_forces() {
    m_list_forces.clear();
}

glm::vec3 MovementBehavior::get_forces() const {
    return m_forces;
}

glm::vec3 MovementBehavior::get_torques() const {
    return m_torques;
}

glm::vec3 MovementBehavior::get_velocity() const {
    return m_velocity;
}

glm::vec3 MovementBehavior::get_acceleration() const {
    return m_acceleration;
}

void MovementBehavior::set_velocity(const glm::vec3 &velocity) {
    m_velocity = velocity;
}

void MovementBehavior::set_angular_velocity(const glm::vec3 &angular_velocity) {
    m_angular_velocity = angular_velocity;
}

void MovementBehavior::set_acceleration(const glm::vec3 &acceleration) {
    m_acceleration = acceleration;
}

glm::vec3 MovementBehavior::get_angular_velocity() const {
    return m_angular_velocity;
}

glm::vec3 MovementBehavior::get_angular_acceleration() const {
    return m_angular_acceleration;
}

bool MovementBehavior::is_translatable() const {
    return m_translatable;
}

bool MovementBehavior::is_rotatable() const {
    return m_rotatable;
}
