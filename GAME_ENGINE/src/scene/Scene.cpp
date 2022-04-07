//
// Created by mariusjenin on 05/03/2022.
//

#include "Scene.hpp"

using namespace scene;

void Scene::draw() {
    m_root->draw(m_cameras.at(m_camera_index)->get_position_in_world());
}

Scene::~Scene() {
    for (auto m_camera: m_cameras) {
        delete m_camera;
    }
    m_cameras.clear();
    for (auto m_node: m_nodes) {
        delete m_node;
    }
    m_nodes.clear();
    delete m_root;
}

void Scene::update(GLFWwindow *window, float delta_time) {
    process_input(window, delta_time);

    //CAMERA
    CameraNodeSG *camera_node = m_cameras[m_camera_index];
    camera_node->update_view_mat();
    camera_node->update_view_pos();
}

Scene::Scene(const std::string &vertex_shader_path, const std::string &fragment_shader_path) {
    m_shaders = new Shaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());
    GLuint program_id = m_shaders->get_program_id();
    glUseProgram(program_id);

    m_camera_index = 0;

    //ROOT
    m_root = new RootSG(m_shaders, ROOT_NAME);

    //LOCATION DATA
    m_shaders->load_location();

    //UNIFORM DEFINED VALUES
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
    shader_data_manager->load_material_const(program_id);
    shader_data_manager->load_lights_const(program_id);
//    m_shaders->load_type_light_to_shader();

}

void Scene::setup() {
    //COMPUTE ALL TRASNFORMATIONS
    m_root->compute_trsf_scene_graph();
    //LOAD LIGHTS
    m_shaders->get_shader_data_manager()->load_lights(m_shaders->get_program_id(), m_lights);
}

Shaders *Scene::get_shaders() const {
    return m_shaders;
}
