//
// Created by mariusjenin on 07/05/2022.
//

#include "RigidBodyBehavior.hpp"

rigid_body_behavior::RigidBodyBehavior_TYPE rigid_body_behavior::RigidBodyBehavior::get_type() const {
    return m_type;
}

void RigidBodyBehavior::set_rigid_body(RigidBodyVolume *rbv) {
    m_rigid_body = rbv;
}
