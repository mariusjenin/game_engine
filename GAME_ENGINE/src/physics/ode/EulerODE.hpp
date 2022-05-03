//
// Created by mariusjenin on 02/05/2022.
//

#ifndef GAME_ENGINE_EULERODE_H
#define GAME_ENGINE_EULERODE_H


#include "ODE.hpp"

namespace physics {
    namespace ode {
        ///Euler Ordinary Differential Equation
        class EulerODE : public ODE {
        public:
            void update(RigidBodyVolume *rbv,float delta_time) override;
        };
    }
}


#endif //GAME_ENGINE_EULERODE_H
