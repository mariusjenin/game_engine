//
// Created by mariusjenin on 02/05/2022.
//

#ifndef GAME_ENGINE_ODEFACTORY_H
#define GAME_ENGINE_ODEFACTORY_H

#include "ODE.hpp"

namespace physics {
    namespace ode {
//        class ODE;
        /// Factory of ODE
        class ODEFactory {
        public:

            /**
             * Create an ODE according to the type given
             * @param ode_type
             * @return ode
             */
            static ODE *generate_ode(ODE_TYPE ode_type);
        };
    }
}

#endif //GAME_ENGINE_ODEFACTORY_H
