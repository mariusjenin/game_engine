//
// Created by mariusjenin on 02/05/2022.
//

#include "ODE.hpp"
#include <src/physics/RigidBodyVolume.hpp>

using namespace physics::ode;

ODE_TYPE ODE::get_type() const {
    return m_type;
}
