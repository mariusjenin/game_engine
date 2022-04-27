//
// Created by mariusjenin on 27/04/2022.
//

#ifndef GAME_ENGINE_FORCE_H
#define GAME_ENGINE_FORCE_H

#include "src/physics/RigidBodyVolume.hpp"

namespace physics{
    namespace force{
        /// Represent a Force to apply on a RigidBodyVolume (Abstract)
        class Force {
        public:
            virtual void apply(RigidBodyVolume* rbv) = 0;
        };
    }
}


#endif //GAME_ENGINE_FORCE_H
