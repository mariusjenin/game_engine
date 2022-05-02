//
// Created by mariusjenin on 27/04/2022.
//

#ifndef GAME_ENGINE_GRAVITYFORCE_H
#define GAME_ENGINE_GRAVITYFORCE_H

#include "Force.hpp"
#include "src/physics/RigidBodyVolume.hpp"

namespace physics {
    namespace force {
        ///Force of the gravity
        class GravityForce : public Force {
            constexpr static const float EARTH_GRAVITY = 9.80665f;
        private:
            float m_gravity;
            glm::vec3 m_direction;
        public:
            explicit GravityForce(glm::vec3 dir = {0,-1,0},float g = EARTH_GRAVITY);
            void apply(RigidBodyVolume* rbv) override;
        };
    }
}

#endif //GAME_ENGINE_GRAVITYFORCE_H
