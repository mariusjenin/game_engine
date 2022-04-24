//
// Created by mariusjenin on 07/04/2022.
//

#include "BoundingBox.hpp"

using namespace physics;

BB_TYPE BoundingBox::get_type() const {
    return m_type;
}

const glm::vec3 &BoundingBox::get_position() const {
    return m_position;
}

void BoundingBox::set_position(const glm::vec3 &position) {
    m_position = position;
}
