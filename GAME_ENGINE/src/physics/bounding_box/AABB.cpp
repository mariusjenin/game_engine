//
// Created by mariusjenin on 07/04/2022.
//

#include "AABB.hpp"
#include "OBB.hpp"
#include "SphereBB.hpp"
#include "src/physics/Collision.hpp"
#include <src/utils/printer.hpp>

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

glm::vec3 AABB::get_min() {
    return m_position - m_size;
}

glm::vec3 AABB::get_max() {
    return m_position + m_size;
}


AABB* AABB::to_AABB() const {
    return new AABB(*this);
}

void AABB::apply_transform(glm::mat4 matrix) {
    std::vector<glm::vec3> positions_with_size = {};
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(m_size.x,m_size.y,m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(m_size.x,m_size.y,-m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(m_size.x,-m_size.y,m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(m_size.x,-m_size.y,-m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(-m_size.x,m_size.y,m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(-m_size.x,m_size.y,-m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(-m_size.x,-m_size.y,m_size.z),1)));
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(-m_size.x,-m_size.y,-m_size.z),1)));
    RCBB::compute(positions_with_size);
}

std::vector<glm::vec3> AABB::to_vertices() const {
    glm::vec3 vec_add = {m_position.x+m_size.x,m_position.y+m_size.y,m_position.z+m_size.z};
    glm::vec3 vec_substract = {m_position.x-m_size.x,m_position.y-m_size.y,m_position.z-m_size.z};
    return {
            {vec_add[0],vec_add[1],vec_add[2]},
            {vec_add[0],vec_add[1],vec_substract[2]},
            {vec_add[0],vec_substract[1],vec_add[2]},
            {vec_add[0],vec_substract[1],vec_substract[2]},
            {vec_substract[0],vec_add[1],vec_add[2]},
            {vec_substract[0],vec_add[1],vec_substract[2]},
            {vec_substract[0],vec_substract[1],vec_add[2]},
            {vec_substract[0],vec_substract[1],vec_substract[2]},
    };
}

AABB::AABB(const AABB &aabb)  : RCBB(aabb) {
    m_size = aabb.m_size;
    m_position = aabb.m_position;
    m_type = AABB_TYPE;
}

glm::mat3 AABB::get_orientation() const {
    return glm::mat3(1);
};

float AABB::get_max_dist(){
    return RCBB::get_max_dist();
}


