//
// Created by mariusjenin on 24/04/2022.
//

#include "RCBB.hpp"
#include "AABB.hpp"

using namespace physics;
using namespace physics::bounding_box;

void RCBB::compute(std::vector<glm::vec3> vertices) {
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
    m_size = (bb.second - bb.first) / 2.f;
}

glm::vec3 RCBB::get_min() {
    return m_position - m_size;
}

glm::vec3 RCBB::get_max() {
    return m_position + m_size;
}

AABB* RCBB::to_AABB() {
    auto* aabb = new AABB(m_position,m_size);
    return aabb;
}

