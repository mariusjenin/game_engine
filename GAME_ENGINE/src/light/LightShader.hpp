#ifndef GAME_ENGINE_LIGHSHADER_H
#define GAME_ENGINE_LIGHSHADER_H

#include "../shader/Shaders.hpp"

using namespace shader;

namespace light {
    /**
     * %Light in the shader based on data retrieved from Light objects
     */
    struct LightShader {
        int type{};
        glsl_vec3 ambient{};
        glsl_vec3 diffuse{};
        glsl_vec3 specular{};
        //Positionned Light
        glsl_vec3 position{};
        float constant_attenuation{};
        float linear_attenuation{};
        float quadratic_attenuation{};
        //Directed Light
        glsl_vec3 direction{};
        //SpotLight
        float inner_cut_off{};
        float outer_cut_off{}; // if inner == outer then no smooth edge
    };

}
#endif //GAME_ENGINE_LIGHSHADER_H
