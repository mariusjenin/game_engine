//
// Created by mariusjenin on 07/04/2022.
//

#include "AABB.hpp"
#include "src/physics/Collision.hpp"

using namespace physics;

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


