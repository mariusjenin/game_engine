//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_SPOTLIGHTBEHAVIOR_H
#define GAME_ENGINE_SPOTLIGHTBEHAVIOR_H


#include "src/light/light_behavior/LightBehavior.hpp"
#include "src/light/Light.hpp"

namespace light {

    namespace behavior {
        /// Behavior of a SpotLight
        class SpotLightBehavior : public LightBehavior {
        private:
            float m_inner_cut_off;
            float m_outer_cut_off; // if inner == outer then no smooth edge
        public:
            /**
             * Constructor of a SpotLightBehavior with the inner and outer cut off
             * @param icoa
             * @param ocoa
             */
            explicit SpotLightBehavior(float icoa, float ocoa);

            void apply_to(LightShader *light_shader) override;
        };
    }
}

#endif //GAME_ENGINE_SPOTLIGHTBEHAVIOR_H
