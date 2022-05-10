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
        public:
             
            /**
             * Constructor of a DirectionLightBehavior
             */
            explicit DirectionLightBehavior();

            void apply_to(LightInfo *light_shader, glm::mat4 model_mat) override;
        };
    }
}

#endif //GAME_ENGINE_DIRECTIONLIGHTBEHAVIOR_H
