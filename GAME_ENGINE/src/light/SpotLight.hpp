//
// Created by mariusjenin on 07/03/2022.
//

#ifndef GAME_ENGINE_SPOTLIGHT_HPP
#define GAME_ENGINE_SPOTLIGHT_HPP


#include "PositionLight.hpp"
#include "DirectionLight.hpp"

namespace light {
    /// Light with a direction, a position and angles of diffusion
    class SpotLight : public DirectionLight, public PositionLight {
    private:
        float m_inner_cut_off;
        float m_outer_cut_off; // if inner == outer then no smooth edge
    public:
        /**
         * Constructor of a Spot Light
         * @param ambient
         * @param diffuse
         * @param specular
         * @param direction
         * @param inner_cut_off_angle
         * @param outer_cut_off_angle
         * @param constant_attenuation
         * @param linear_attenuation
         * @param quadratic_attenuation
         */
        explicit SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction,
                           float inner_cut_off_angle,
                           float outer_cut_off_angle, float constant_attenuation = 0.f, float linear_attenuation = 0.f,
                           float quadratic_attenuation = 0.f);

        void to_light_shader(LightShader *light_struct) override;

        bool positionned_light() override;
    };
}


#endif //GAME_ENGINE_SPOTLIGHT_HPP
