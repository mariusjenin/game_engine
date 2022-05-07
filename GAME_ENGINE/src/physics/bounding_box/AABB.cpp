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

glm::vec3 AABB::get_min() {
    return m_position - m_size;
}

glm::vec3 AABB::get_max() {
    return m_position + m_size;
}


AABB* AABB::to_AABB() const {
    return new AABB(*this);
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
    positions_with_size.emplace_back(glm::vec3(matrix * glm::vec4(m_position + glm::vec3(m_size.x,m_size.y,m_size.z),1)));
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

bool AABB::is_point_in(glm::vec3 point) const {
    glm::vec3 distance = point - m_position;
    for (int i = 0; i < 3; ++i) {
        if (distance[i] > m_size[i]) {
            return false;
        }
        if (distance[i] < -m_size[i]) {
            return false;
        }
    }
    return true;
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

float AABB::is_intersected(Ray){
    return -1.f;
}

glm::vec3 AABB::closest_point(glm::vec3 pt) const {
    return glm::vec3(0.f);
};


