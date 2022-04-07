//
// Created by mariusjenin on 07/03/2022.
//

#ifndef GAME_ENGINE_DIRECTIONLIGHT_HPP
#define GAME_ENGINE_DIRECTIONLIGHT_HPP


#include "Light.hpp"


namespace light {

    /// Light with a direction
    class DirectionLight : public virtual Light {
    private:
        glm::vec3 m_direction;
    public:
        /**
         * Construct a Direction Light (A light with a direction)
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
