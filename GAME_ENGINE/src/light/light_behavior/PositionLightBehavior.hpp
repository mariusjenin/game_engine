//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_POSITIONLIGHTBEHAVIOR_H
#define GAME_ENGINE_POSITIONLIGHTBEHAVIOR_H


#include "LightBehavior.hpp"
#include "src/light/Light.hpp"

namespace light {

    namespace behavior {
        /// Behavior of a positionned Light
        class PositionLightBehavior : public LightBehavior {
        private:
            float m_constant_attenuation;
            float m_linear_attenuation;
            float m_quadratic_attenuation;
        public:
            /**
             * Constructor if a PositionLightBehavior with the 3 attenuations
             * @param ca
             * @param la
             * @param qa
             */
            explicit PositionLightBehavior(float ca, float la, float qa);

            void apply_to(LightShader *light_shader) override;
        };
    }

}

#endif //GAME_ENGINE_POSITIONLIGHTBEHAVIOR_H
