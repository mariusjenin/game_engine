//
// Created by mariusjenin on 02/05/2022.
//

#ifndef GAME_ENGINE_ODE_H
#define GAME_ENGINE_ODE_H

#include <glm/glm.hpp>

namespace physics{
    class RigidBodyVolume;
    namespace ode{

        /**
         * Enum of the different types of ODE
         */
        enum ODE_TYPE {
            EULER_TYPE,
            VERLET_TYPE,
            RK4_TYPE,
        };

        ///Ordinary differential equation
        class ODE {
        public:
            virtual void update(RigidBodyVolume *rbv,float delta_time) = 0;
        };
    }
}

#endif //GAME_ENGINE_ODE_H
