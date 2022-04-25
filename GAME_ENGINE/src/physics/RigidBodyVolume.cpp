#include "RigidBodyVolume.hpp"
#include "Collision.hpp"
#include "src/utils/printer.hpp"
#include <glm/gtx/matrix_decompose.hpp>

using namespace physics;
using namespace scene_graph;

RigidBodyVolume::RigidBodyVolume(NodeGameSG *ng,float mass, float friction, float cor) {
    m_node_game = ng;
    m_mass = mass;
    m_friction = friction;
    m_cor = cor;
}

RigidBodyVolume::~RigidBodyVolume() = default;

void RigidBodyVolume::update(float delta_time) {
    float damping = 0.98f;
    glm::vec3 acceleration = m_forces * inverse_mass();

    m_velocity += acceleration * delta_time;
    m_velocity *= damping;

    // recup la matrice monde du noeud
    // Appliquer le mouvement
    // calculer l'inverse de la matrice resultante
    // retrouver la translation et la rotation

//    m_position += m_velocity * delta_time;

    glm::vec3 translation_modification_world = m_velocity * delta_time;
    //TODO
//    //Get the local to world matrix
//    glm::mat4 mat_world = ((NodeSG *) m_node_game)->get_matrix_recursive_local();
//    //inverse it
//    glm::mat4 inv_mat_world = glm::inverse(mat_world);
//    print_mat4(mat_world);
//    print_mat4(inv_mat_world);
//
//    //Get the translation modification and get it in the local space of the node
//    Transform local_trsf = Transform();
//    local_trsf.set_matrix(inv_mat_world);
//    glm::vec3 translation_modification = local_trsf.apply_to_point(translation_modification_world);
//    //Apply this modification
//    glm::vec3 translation_base = ((NodeSG *) m_node_game)->get_trsf()->get_translation();
//
//
//    print_mat4(mat_world);
//    print_mat4(inv_mat_world);
//
//        std::cout<< m_node_game->get_bb()->get_position()[1]<< std::endl;
    ((NodeSG *) m_node_game)->get_trsf()->set_translation(
            ((NodeSG *) m_node_game)->get_trsf()->get_translation() + translation_modification_world);
    ((NodeSG *) m_node_game)->get_trsf()->compute();
}

void RigidBodyVolume::apply_forces() {
    m_forces = glm::vec3(0., -9.81f, 0.) * m_mass;
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
//                    collision = PhysicsGeometry::get_data_collision((SphereBB &) *bb1, (SphereBB &) *bb2);
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

    if (inv_ma + inv_mb == 0.f)
        return;

    glm::vec3 rel_velocity = rbv.m_velocity - m_velocity;
    glm::vec3 rel_normal = glm::normalize(collision.normal);

    //Les objets s'éloignent
    if (glm::dot(rel_normal, rel_velocity) > 0.f)
        return;

    float bounce = fminf(m_cor, rbv.m_cor);
    float numerator = (-(1.f + bounce) * glm::dot(rel_normal, rel_velocity));
    float j = numerator / (inv_ma + inv_mb);
    if (collision.contacts.size() >= 0 && j != 0.f) {
        j /= (float) collision.contacts.size();
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
    if (collision.contacts.size() >= 0 && jt != 0.f) {
        jt /= (float) collision.contacts.size();
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

NodeGameSG *RigidBodyVolume::get_node() {
    return m_node_game;
}