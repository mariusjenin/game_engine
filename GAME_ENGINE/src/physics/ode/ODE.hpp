//
// Created by mariusjenin on 02/05/2022.
//

#ifndef GAME_ENGINE_ODE_H
#define GAME_ENGINE_ODE_H

#include <glm/glm.hpp>

namespace physics{
    class RigidBodyVolume;
    class PhysicsSystem;
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
        protected:
            ODE_TYPE m_type{};
        public:

            /**
             * Update position and rotation of a RigidBodyVolume with an ODE equation
             * @param rbv
             * @param delta_time
             */
            virtual void update(RigidBodyVolume *rbv, float delta_time) = 0;

            /**
             * Getter of the type of the ODE
             * @return type
             */
            ODE_TYPE get_type() const;
        };
    }
}

#endif //GAME_ENGINE_ODE_H
