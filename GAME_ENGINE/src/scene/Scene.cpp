//
// Created by mariusjenin on 05/03/2022.
//

#include "Scene.hpp"

using namespace scene;

void Scene::render() {
    m_root->draw(m_cameras[m_camera_index]->get_position_in_world());
}

Scene::~Scene() {

    glDeleteProgram(m_shaders->get_program_id());

    delete m_root;
}

void Scene::update(GLFWwindow *window, float delta_time) {
    //CAMERA
    NodeGameSG *camera_node = m_cameras[m_camera_index];
    camera_node->update_view_mat();
    camera_node->update_view_pos();

    process_input(window, delta_time);
}

Scene::Scene(const std::string &vertex_shader_path, const std::string &fragment_shader_path) {
    m_shaders = new Shaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());
    GLuint program_id = m_shaders->get_program_id();
    glUseProgram(program_id);

    m_camera_index = 0;

    //ROOT
    m_root = new RootSG(m_shaders);

    //LOCATION DATA
    m_shaders->load_location();

    //UNIFORM DEFINED VALUES
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
    shader_data_manager->load_material_const(program_id);
    shader_data_manager->load_lights_const(program_id);
    shader_data_manager->load_debug_const(program_id);

}

void Scene::setup() {
    //COMPUTE ALL TRANSFORMATIONS
    m_root->compute_trsf_scene_graph();

    //LOAD LIGHTS
    size_t size_lights = m_lights.size();
    LightShader lights_struct_array[size_lights];
    int i = 0;
    for (const auto &light: m_lights) {
        lights_struct_array[i] = light->generate_light_struct();
        i++;
    }
    m_shaders->get_shader_data_manager()->load_lights(m_shaders->get_program_id(), lights_struct_array,(int)size_lights);
}

Shaders *Scene::get_shaders() const {
    return m_shaders;
}

void Scene::update_physics(GLFWwindow *window, float delta_time) {
    if(m_physics_system != nullptr){
        NodeGameSG *camera_node = m_cameras[m_camera_index];
        m_physics_system->update(camera_node->get_position_in_world(),delta_time);
    }
}
