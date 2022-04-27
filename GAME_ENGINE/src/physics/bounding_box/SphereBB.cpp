//
// Created by mariusjenin on 24/04/2022.
//

#include <iostream>
#include "SphereBB.hpp"
#include "AABB.hpp"

using namespace physics;
using namespace physics::bounding_box;

SphereBB::SphereBB() {
    m_type = SphereBB_TYPE;
}

void SphereBB::compute(std::vector<glm::vec3> vertices) {
    std::pair<glm::vec3, glm::vec3> bb;
    bb.first = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    bb.second = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (auto &vertex_position: vertices) {
        if (vertex_position[0] < bb.first[0]) bb.first[0] = vertex_position[0];
        if (vertex_position[1] < bb.first[1]) bb.first[1] = vertex_position[1];
        if (vertex_position[2] < bb.first[2]) bb.first[2] = vertex_position[2];
        if (vertex_position[0] > bb.second[0]) bb.second[0] = vertex_position[0];
        if (vertex_position[1] > bb.second[1]) bb.second[1] = vertex_position[1];
        if (vertex_position[2] > bb.second[2]) bb.second[2] = vertex_position[2];
    }

    m_position = (bb.first + bb.second) / 2.f;
    glm::vec3 radius = bb.second - m_position;
    m_radius = std::max(radius.x, std::max(radius.y, radius.z));
}

float SphereBB::get_radius() const {
    return m_radius;
}

AABB *SphereBB::to_AABB() {
    return new AABB(m_position,{m_radius,m_radius,m_radius});
}

Collision SphereBB::get_data_collision(const SphereBB &bb) {
    Collision collision;

    float sum_radius = this->get_radius() + bb.get_radius();
    glm::vec3 dist = bb.get_position() - this->get_position();

    if ((glm::length(dist) * glm::length(dist) - sum_radius * sum_radius) > 0.f || glm::length(dist) == 0.f)
        return collision;                  // no collision

    dist = glm::normalize(dist);

    collision.colliding = true;
    collision.normal = dist;
    collision.depth = fabsf(glm::length(dist) - sum_radius) * 0.5f;

    float dist_to_inter = this->get_radius() - collision.depth;
    glm::vec3 contact_pt = this->get_position() + dist * dist_to_inter;

    collision.contacts.push_back(contact_pt);

    return collision;
}

Collision SphereBB::get_data_collision(const AABB &bb) {
    return {}; //TODO
}

Collision SphereBB::get_data_collision(const OBB &bb) {
    return {}; //TODO
}

void SphereBB::apply_transform(glm::mat4 matrix) {
    glm::vec3 position_radius = glm::vec3(matrix * glm::vec4(m_position + glm::vec3(m_radius,m_radius,m_radius),1));
    m_position = glm::vec3(matrix * glm::vec4(m_position,1));
    glm::vec3 diff = m_position-position_radius;
    m_radius = std::max(abs(diff.x),std::max(abs(diff.y),abs(diff.z)));
}

