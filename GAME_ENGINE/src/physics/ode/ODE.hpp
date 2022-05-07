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

        ///Ordinary Differential Equation (Abstract) 
        class ODE {
        public:
            /**
             * Update position and rotation of a RigidBodyVolume with an ODE equation
             * @param rbv
             * @param delta_time
             */
            virtual void update(RigidBodyVolume *rbv,float delta_time, bool use_angular = true) = 0;
        };
    }
}

#endif //GAME_ENGINE_ODE_H
