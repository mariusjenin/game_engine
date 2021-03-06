//
// Created by mariusjenin on 07/03/2022.
//

#ifndef GAME_ENGINE_POSITIONLIGHT_HPP
#define GAME_ENGINE_POSITIONLIGHT_HPP


#include "Light.hpp"
#include "src/light/light_behavior/PositionLightBehavior.hpp"

namespace light {
    /// Light with a position and whose intensity can be attenuated
    class PositionLight : public virtual Light {
    public:
        /**
         * Constructor of a Positionned Light (has a PositionLightBehavior)
         * @param ambient
         * @param diffuse
         * @param specular
         * @param constant_attenuation
         * @param linear_attenuation
         * @param quadratic_attenuation
         */
        explicit PositionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                               float constant_attenuation = 0.f,
                               float linear_attenuation = 0.f, float quadratic_attenuation = 0.f);

        void to_light_info(LightInfo *light_shader, glm::mat4 model_mat) override;
    };
}

#endif //GAME_ENGINE_POSITIONLIGHT_HPP
