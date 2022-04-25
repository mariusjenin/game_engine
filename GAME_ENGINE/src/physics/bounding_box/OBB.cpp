//
// Created by mariusjenin on 24/04/2022.
//

#include "OBB.hpp"
#include "src/physics/Collision.hpp"

using namespace physics;

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
