//
// Created by mariusjenin on 07/03/2022.
//

#ifndef GAME_ENGINE_DIRECTIONLIGHT_HPP
#define GAME_ENGINE_DIRECTIONLIGHT_HPP


#include "src/light/Light.hpp"
#include "src/light/light_behavior/DirectionLightBehavior.hpp"


namespace light {
    /// Light with a direction
    class DirectionLight : public virtual Light {
    public:
        /**
         * Construct a Direction Light (has a DirectionLightBehavior)
         * @param ambient
         * @param diffuse
         * @param specular
         * @param direction
         */
        explicit DirectionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);

        void to_light_shader(LightShader *light_struct) override;
    };
}

#endif //GAME_ENGINE_DIRECTIONLIGHT_HPP
