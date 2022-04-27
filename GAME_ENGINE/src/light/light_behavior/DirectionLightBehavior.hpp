//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_DIRECTIONLIGHTBEHAVIOR_H
#define GAME_ENGINE_DIRECTIONLIGHTBEHAVIOR_H


#include "LightBehavior.hpp"
#include "src/light/Light.hpp"

namespace light {

    namespace behavior {
        /// Behavior of a directed Light
        class DirectionLightBehavior : public LightBehavior {
        private:
            glm::vec3 m_direction;
        public:
            /**
             * Constructor of a DirectionLightBehavior with the direction
             * @param direction
             */
            explicit DirectionLightBehavior(glm::vec3 direction);

            void apply_to(LightShader *light_shader) override;
        };
    }
}

#endif //GAME_ENGINE_DIRECTIONLIGHTBEHAVIOR_H
