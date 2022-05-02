//
// Created by mariusjenin on 02/05/2022.
//

#ifndef GAME_ENGINE_VERLETODE_HPP
#define GAME_ENGINE_VERLETODE_HPP

#include "ODE.hpp"

namespace physics {
    namespace ode {
        ///Verlet Ordinary Differential Equation
        class VerletODE : public ODE {
        public:
            void update(RigidBodyVolume *rbv,float delta_time) override;
        };
    }
}

#endif //GAME_ENGINE_VERLETODE_HPP
