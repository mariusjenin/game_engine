//
// Created by mariusjenin on 06/03/2022.
//

#include "Material.hpp"

using namespace material;

Material::Material(Shaders *shaders, float shininess) {
    m_shininess = shininess;
    m_shaders = shaders;
}

void Material::load_in_shader() {
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
    glUniform1f(shader_data_manager->get_location(ShadersDataManager::MATERIAL_SHININESS_LOC_NAME), m_shininess);
}