//
// Created by mariusjenin on 24/04/2022.
//

#include "OBB.hpp"
#include "src/physics/Collision.hpp"

using namespace physics;
using namespace physics::bounding_box;

OBB::OBB() {
    m_type = OBB_TYPE;
}

Collision OBB::get_data_collision(const SphereBB &bb) {
    return {}; //TODO
}

Collision OBB::get_data_collision(const AABB &bb) {
    return {}; //TODO
}

Collision OBB::get_data_collision(const OBB &bb) {
    return {}; //TODO
}

void OBB::apply_transform(glm::mat4 matrix) {
    glm::vec3 position_with_size = glm::vec3(matrix * glm::vec4(m_position + m_size,1));
    m_position = glm::vec3(matrix * glm::vec4(m_position,1));
    m_size = m_size - m_position;
}
