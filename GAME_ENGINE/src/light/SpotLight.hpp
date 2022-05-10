//
// Created by mariusjenin on 07/03/2022.
//

#ifndef GAME_ENGINE_SPOTLIGHT_HPP
#define GAME_ENGINE_SPOTLIGHT_HPP


#include "Light.hpp"
#include "src/light/light_behavior/SpotLightBehavior.hpp"
#include "src/light/light_behavior/PositionLightBehavior.hpp"
#include "src/light/light_behavior/DirectionLightBehavior.hpp"

namespace light {
    /// Light with a direction, a position and angles of diffusion
    class SpotLight : public Light {
    public:

        /**
         * Constructor of a Spot Light (has a DirectionLightBehavior, a PositionLightBehavior and a SpotLightBehavior)
         * @param ambient
         * @param diffuse
         * @param specular
         * @param id_texture_shadow_map
         * @param inner_cut_off_angle
         * @param outer_cut_off_angle
         * @param resol
         * @param z_near
         * @param z_far
         * @param constant_attenuation
         * @param linear_attenuation
         * @param quadratic_attenuation
         */
        explicit SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLuint id_texture_shadow_map, float inner_cut_off_angle = 20.f, float outer_cut_off_angle = 25.f, float resol = 1000, float z_near=1.0f, float z_far = 1000.f,
                           float constant_attenuation = 0.f, float linear_attenuation = 0.f,
                           float quadratic_attenuation = 0.f);

        void to_light_info(LightInfo *light_struct, glm::mat4 model_mat) override;
    };
}


#endif //GAME_ENGINE_SPOTLIGHT_HPP
