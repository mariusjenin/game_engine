//
// Created by mariusjenin on 06/03/2022.
//

#include "MaterialColor.hpp"

#include <utility>

using namespace material;

MaterialColor::MaterialColor(Shaders *shaders, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                             float shininess) : Material(shaders, shininess) {
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
}

MaterialColor::MaterialColor(Shaders *shaders, glm::vec3 color, float shininess) : Material(shaders, shininess) {
    m_ambient = color;
    m_diffuse = color;
    m_specular = color;
}

void MaterialColor::load_in_shader() {
    Material::load_in_shader();

    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
    glUniform1i(shader_data_manager->get_location(ShadersDataManager::MATERIAL_TYPE_LOC_NAME), MATERIAL_TYPE_COLOR);
    glUniform3fv(shader_data_manager->get_location(ShadersDataManager::MATERIAL_AMBIENT_LOC_NAME), 1, &m_ambient[0]);
    glUniform3fv(shader_data_manager->get_location(ShadersDataManager::MATERIAL_DIFFUSE_LOC_NAME), 1, &m_diffuse[0]);
    glUniform3fv(shader_data_manager->get_location(ShadersDataManager::MATERIAL_SPECULAR_LOC_NAME), 1, &m_specular[0]);
}
