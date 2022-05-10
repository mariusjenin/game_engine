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
         */
        explicit DirectionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

        void to_light_info(LightInfo *light_struct, glm::mat4 model_mat) override;
    };
}

#endif //GAME_ENGINE_DIRECTIONLIGHT_HPP
