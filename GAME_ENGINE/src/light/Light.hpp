//
// Created by mariusjenin on 07/03/2022.
//

#ifndef GAME_ENGINE_LIGHT_HPP
#define GAME_ENGINE_LIGHT_HPP

#include <GL/glew.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

// Include GLM
#include <glm/glm.hpp>

#include "LightShader.hpp"
#include "src/light/light_behavior/LightBehavior.hpp"

using namespace light::behavior;

namespace light {

    /// Base %Light (Abstract)
    class Light {
    private:
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
    protected:
        std::vector<LightBehavior *> m_light_behaviors;

        /**
             * Constructor of the base Light (with ambient, diffuse and specular components)
             * it is protected because we don't want this class to be instantiated
             * @param ambient
             * @param diffuse
             * @param specular
             */
        explicit Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    public:
        const static int LIGHT_TYPE_DIRECTIONAL = 0;
        const static int LIGHT_TYPE_POINT = 1;
        const static int LIGHT_TYPE_SPOT = 2;

        /**
         * fill the data into a LightShader
         * @param light_shader
         */
        virtual void to_light_shader(LightShader *light_shader);

        /**
         * Getter of if the Light has a position
         * @return is_positionned
         */
        virtual bool positionned_light();

    };

}
#endif //GAME_ENGINE_LIGHT_HPP
