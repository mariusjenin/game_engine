#ifndef GAME_ENGINE_LIGHSHADER_H
#define GAME_ENGINE_LIGHSHADER_H

#include <src/shader/ShadowMap.hpp>
#include "src/shader/Shaders.hpp"

using namespace shader;

namespace light {

    ///Light Information
    struct LightInfo {
        int type{};
        glm::vec3 ambient{};
        glm::vec3 diffuse{};
        glm::vec3 specular{};
        //Positionned Light
        glm::vec3 position{};
        float constant_attenuation{};
        float linear_attenuation{};
        float quadratic_attenuation{};
        //Directed Light
        glm::vec3 direction{};
        //SpotLight
        float inner_cut_off{};
        float outer_cut_off{}; // if inner == outer then no smooth edge
        //Depth and Shadow Map
        bool generate_depth_map{};
        int index_depth_map{};
//        float bias_depth_map{};
        glm::mat4 depth_vp_mat;
        ShadowMap* shadow_map;

        /**
         * Load the depth View and Projection Matrix
         * @param shaders
         */
        void load_depth_vp_matrix(Shaders* shaders);
    };

    ///%Light in the shader based on data retrieved from Light objects
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
        //Depth and Shadow Map
        int generate_depth_map{};
        int index_sampler_depth_map{};
//        float bias_depth_map{};
        glsl_mat4 depth_vp_mat{};

        /**
         * Constructor of a LightSHader with LightInfo
         * @param li
         */
        explicit LightShader(LightInfo li);

        /**
         * Empty COnstrcutor of LightShader
         */
        LightShader();
    };
}
#endif //GAME_ENGINE_LIGHSHADER_H
