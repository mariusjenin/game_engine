//
// Created by mariusjenin on 02/05/2022.
//

#include "ODEFactory.hpp"
#include "EulerODE.hpp"
#include "VerletODE.hpp"
#include "RungeKutta4ODE.hpp"

using namespace physics::ode;

ODE *ODEFactory::generate_ode(ode::ODE_TYPE ode_type) {
    switch (ode_type) {
        case EULER_TYPE:
            return new EulerODE();
        case VERLET_TYPE:
            return new VerletODE();
        case RK4_TYPE:
            return new RungeKutta4ODE();
        default:
            return nullptr;
    }
}
