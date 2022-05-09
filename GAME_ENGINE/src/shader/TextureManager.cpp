//
// Created by mariusjenin on 09/05/2022.
//

#include "TextureManager.hpp"

using namespace shader;

TextureManager::TextureManager(shader::ShadersDataManager *shader_data_manager) {
    m_shader_data_manager = shader_data_manager;
    m_next_id_texture = 0;
}

GLuint TextureManager::load_uniform_texture(GLuint program_id, const std::string &name, const std::string &path) {
    m_shader_data_manager->load_custom_uniform_location(program_id, name);
    GLint id_texture = load_texture(path);;
    glUniform1i(m_shader_data_manager->get_location(name), id_texture);
    return id_texture;
}

GLint TextureManager::load_texture(const std::string &path) {
    GLint id_texture = m_next_id_texture;
    load_bmp_custom(path, id_texture);
    increment_id_texture();
    return id_texture;
}

void TextureManager::increment_id_texture() {
    m_next_id_texture++;
}

GLint TextureManager::get_next_id_texture() const {
    return m_next_id_texture;
}
