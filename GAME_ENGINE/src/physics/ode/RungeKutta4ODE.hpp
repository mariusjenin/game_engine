//
// Created by mariusjenin on 02/05/2022.
//

#ifndef GAME_ENGINE_RUNGEKUTTA4ODE_H
#define GAME_ENGINE_RUNGEKUTTA4ODE_H

#include "ODE.hpp"

namespace physics {
    namespace ode {
        class RungeKutta4ODE : public ODE {
        public:
            void update(RigidBodyVolume *rbv,float delta_time) override;
        };
    }
}


#endif //GAME_ENGINE_RUNGEKUTTA4ODE_H