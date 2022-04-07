//
// Created by mariusjenin on 07/03/2022.
//

#ifndef GAME_ENGINE_LIGHT_HPP
#define GAME_ENGINE_LIGHT_HPP

#include <GL/glew.h>

#include <cstdio>
#include <cstdlib>
#include <string>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../shader/Shaders.hpp"
using namespace shader;

namespace light {

    /**
     * %Light in the shader based on data retrieved from Light objects
     */
    struct LightShader {
        int type{};
        glsl_vec3 ambient;
        glsl_vec3 diffuse;
        glsl_vec3 specular;
        //Positionned Light
        glsl_vec3 position;
        float constant_attenuation{};
        float linear_attenuation{};
        float quadratic_attenuation{};
        //Directed Light
        glsl_vec3 direction;
        //SpotLight
        float inner_cut_off{};
        float outer_cut_off{}; // if inner == outer then no smooth edge
    };


    /// Base %Light (Abstract)
    class Light {
    protected:
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;

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
