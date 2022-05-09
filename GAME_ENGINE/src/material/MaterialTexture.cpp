//
// Created by mariusjenin on 06/03/2022.
//

#include "MaterialTexture.hpp"

#include <utility>

using namespace material;

MaterialTexture::MaterialTexture(Shaders *shaders, TextureManager* texture_manager, const std::string &path_diffuse_texture,
                                 const std::string &path_specular_texture, float shininess)
        : Material(shaders, shininess) {
    m_id_diffuse_texture = texture_manager->load_texture(path_diffuse_texture);
    m_id_specular_texture = texture_manager->load_texture(path_specular_texture);
}

MaterialTexture::MaterialTexture(Shaders *shaders, int id_diffuse_texture, int id_specular_texture, float shininess)
        : Material(shaders, shininess) {
    m_id_diffuse_texture = id_diffuse_texture;
    m_id_specular_texture = id_specular_texture;
}

MaterialTexture::MaterialTexture(Shaders *shaders, int id_texture, float shininess) : Material(shaders, shininess) {
    m_id_diffuse_texture = id_texture;
    m_id_specular_texture = id_texture;
}

void MaterialTexture::load_in_shader() {
    Material::load_in_shader();
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
    glUniform1i(shader_data_manager->get_location(ShadersDataManager::MATERIAL_TYPE_LOC_NAME), MATERIAL_TYPE_TEXTURE);
    glUniform1i(shader_data_manager->get_location(ShadersDataManager::MATERIAL_DIFFUSE_TEXTURE_LOC_NAME),
                m_id_diffuse_texture);
    glUniform1i(shader_data_manager->get_location(ShadersDataManager::MATERIAL_SPECULAR_TEXTURE_LOC_NAME),
                m_id_specular_texture);
}
