//
// Created by mariusjenin on 07/03/2022.
//

#ifndef GAME_ENGINE_POSITIONLIGHT_HPP
#define GAME_ENGINE_POSITIONLIGHT_HPP


#include "Light.hpp"

namespace light {
    /// Light with a position and whose intensity can be attenuated
    class PositionLight : public virtual Light {
    private:
        float m_constant_attenuation;
        float m_linear_attenuation;
        float m_quadratic_attenuation;
    public:
        /**
         * Constructor of a Positionned Light
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

        void to_light_shader(LightShader *light_shader) override;

        bool positionned_light() override;
    };
}

#endif //GAME_ENGINE_POSITIONLIGHT_HPP
