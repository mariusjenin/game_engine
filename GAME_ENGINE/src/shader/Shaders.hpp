
#ifndef GAME_ENGINE_SHADERS_HPP
#define GAME_ENGINE_SHADERS_HPP


#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "GL/glew.h"
#include "TextureManager.hpp"

#include <src/shader/ShadersDataManager.hpp>

namespace shader {
    /**
     * Vec3 used in shader
     */
    struct alignas(16) glsl_vec3 {
        float x, y, z;

        glsl_vec3();

        /**
         * Construct a glsl_vec3 from a glm::vec3
         * @param v
         */
        explicit glsl_vec3(glm::vec3 v);
    };

    /// Group the VAOManager and the ShadersDataManager
    class Shaders {
    private:
        GLuint m_program_id;
        ShadersDataManager *m_shader_data_manager;
        TextureManager *m_texture_manager;
    public:
        /**
         * Constructor of the Shaders given paths to the shaders files
         * @param vertex_file_path
         * @param fragment_file_path
         */
        Shaders(const char *vertex_file_path, const char *fragment_file_path);

        /**
         * Getter of the program id
         * @return program_id
         */
        GLuint get_program_id() const;

        /**
         * Getter of the ShadersDataManager
         * @return shader_data_manager
         */
        ShadersDataManager *get_shader_data_manager();

        /**
         * Getter of the TextureManager
         * @return texture_manager
         */
        TextureManager *get_texture_manager();

        /// Load all the locations thanks to the ShadersDataManager
        void load_location() const;

    };
}

#endif //GAME_ENGINE_SHADERS_HPP
