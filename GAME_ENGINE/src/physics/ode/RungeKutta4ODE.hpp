//
// Created by mariusjenin on 02/05/2022.
//

#ifndef GAME_ENGINE_RUNGEKUTTA4ODE_H
#define GAME_ENGINE_RUNGEKUTTA4ODE_H

#include "ODE.hpp"

namespace physics {
    namespace ode {
        ///Runge Kutta Ordinary Differential Equation
        class RungeKutta4ODE : public ODE {
        public:
            RungeKutta4ODE();
            void update(RigidBodyVolume *rbv, float delta_time) override;
        };
    }
}


#endif //GAME_ENGINE_RUNGEKUTTA4ODE_H
