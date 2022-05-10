//
// Created by mariusjenin on 09/05/2022.
//

#ifndef GAME_ENGINE_TEXTUREMANAGER_H
#define GAME_ENGINE_TEXTUREMANAGER_H

// Include GLEW
#include <GL/glew.h>
#include "ShadersDataManager.hpp"
#include <src/utils/texture.hpp>

namespace shader {
    ///Manager of all the texture in the shaders
    class TextureManager {
    private:
        GLint m_next_id_texture;
        ShadersDataManager* m_shader_data_manager;
    public:
        /**
         * Constructor of the TextureManager
         * @param shader_data_manager
         */
        explicit TextureManager(ShadersDataManager* shader_data_manager);

        /**
         * Load a new texture in the shader
         * @param program_id
         * @param name
         * @param path
         * @return id_texture
         */
        GLuint load_uniform_texture(GLuint program_id, const std::string &name, const std::string &path);

        /**
         * Load a texture at an existant location in the shader
         * @param path
         * @return id_texture
         */
        GLint load_texture(const std::string &path);

        /**
         * Getter of the next id_texture and increment it
         * @return id_texture
         */
        GLint get_and_increment_id_texture();
    };
}

#endif //GAME_ENGINE_TEXTUREMANAGER_H
