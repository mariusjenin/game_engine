//
// Created by mariusjenin on 07/04/2022.
//

#include "AABB.hpp"
#include "src/physics/Collision.hpp"

using namespace physics;
using namespace physics::bounding_box;

AABB::AABB() {
    m_type = AABB_TYPE;
}

AABB::AABB(glm::vec3 position, glm::vec3 size) {
    m_type = AABB_TYPE;
    m_position = position;
    m_size = size;
}

AABB* AABB::to_AABB() {
    return this;
}

Collision AABB::get_data_collision(const SphereBB &bb) {
    return {}; //TODO
}

Collision AABB::get_data_collision(const AABB &bb) {
    return {}; //TODO
}

Collision AABB::get_data_collision(const OBB &bb) {
    return {}; //TODO
}

void AABB::apply_transform(glm::mat4 matrix) {
    std::vector<glm::vec3> positions_with_size = {};
    positions_with_size.emplace_back(matrix * glm::vec4(m_position + glm::vec3(m_size.x,m_size.y,m_size.z),1));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(m_size.x,m_size.y,-m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(m_size.x,-m_size.y,m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(m_size.x,-m_size.y,-m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(-m_size.x,m_size.y,m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(-m_size.x,m_size.y,-m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(-m_size.x,-m_size.y,m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(-m_size.x,-m_size.y,-m_size.z),1)));
    RCBB::compute(positions_with_size);
    m_position = glm::vec3(matrix * glm::vec4(m_position,1));
}


