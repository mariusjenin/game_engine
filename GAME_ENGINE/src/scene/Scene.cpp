//
// Created by mariusjenin on 05/03/2022.
//

#include <src/shader/ShadowMap.hpp>
#include "Scene.hpp"

using namespace scene;

void Scene::render(bool allow_debug, Shaders* shaders) {
    if(shaders == nullptr) shaders = m_shaders;
    m_root->draw(shaders,m_cameras[m_camera_index]->get_position_in_world(), allow_debug);
}

Scene::~Scene() {

    glDeleteProgram(m_shaders->get_program_id());

    delete m_root;
}

void Scene::update(float delta_time) {
    load_projection_matrix();
    //CAMERA
    NodeGameSG *camera_node = m_cameras[m_camera_index];
    camera_node->update_view_mat(m_shaders);
    camera_node->update_view_pos(m_shaders);
    if(m_physics_system != nullptr){
        m_physics_system->update_bodies(camera_node->get_position_in_world(),delta_time);
    }

    load_lights();

    process_input(delta_time);
}

Scene::Scene(GLFWwindow *window,const std::string &vertex_shader_path, const std::string &fragment_shader_path, float mult_physics) {
    m_window = window;
    m_shaders = new MainShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());
    GLuint program_id = m_shaders->get_program_id();
    m_shaders->use();

    m_camera_index = 0;

    //ROOT
    m_root = new RootSG();

    ShadowMapShaders* shadow_map_shaders = m_shaders->get_shadow_map_shaders();

    //LOCATION DATA
    m_shaders->load_location();
    shadow_map_shaders->load_location();

    //UNIFORM DEFINED VALUES
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
    shader_data_manager->load_material_const(program_id);
    shader_data_manager->load_lights_const(program_id);
    shader_data_manager->load_debug_const(program_id);
}

void Scene::setup() {
    //COMPUTE ALL TRANSFORMATIONS
    m_root->compute_trsf_scene_graph();
}

void Scene::load_lights(){
    ShadowMapShaders* shadow_map_shaders = m_shaders->get_shadow_map_shaders();
    shadow_map_shaders->use();

    size_t size_lights = std::min((int) m_lights.size(),200);

    LightShader lights_struct_array[size_lights];

    GLuint depth_maps[200];
    int count_index_depth_map = 0;

    for(int i = 0 ; i < size_lights; i++){
        LightInfo light_info = m_lights[i]->generate_light_struct();
        depth_maps[i] = light_info.depth_map;
        lights_struct_array[i] = LightShader(light_info);
        //Generate the Shadow if it has to
        if(light_info.generate_shadow_map){
            //Load de depth light VP into the depth pass
            light_info.load_depth_vp_matrix(shadow_map_shaders);
            //Bind the buffer, render the depth map and unbind it
            light_info.shadow_map->activate_texture();
            light_info.shadow_map->bind();
            render(false,shadow_map_shaders);
//            light_info.shadow_map->print_in_img((std::string("../test")+std::to_string(i)+std::string(".ppm")).c_str());
            ShadowMap::unbind_bound_shadow_map();

            lights_struct_array[i].index_depth_map = count_index_depth_map;
            count_index_depth_map++;
        }
    }
    adapt_viewport();
    m_shaders->use();

    //Build the array of shadow maps
    glUniform1uiv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::SHADOW_MAP_ARRAY_LOC_NAME),200, depth_maps);

    m_shaders->get_shader_data_manager()->load_lights(m_shaders->get_program_id(), lights_struct_array,(int)size_lights);

}

Shaders *Scene::get_shaders() const {
    return m_shaders;
}

void Scene::update_physics(float delta_time) {
    if(m_physics_system != nullptr){
        NodeGameSG *camera_node = m_cameras[m_camera_index];
        m_physics_system->update_collisions(camera_node->get_position_in_world(),delta_time);
    }
}

void Scene::load_projection_matrix() {
    int width,height;
    glfwGetWindowSize(m_window, &width, &height);

    //PROJECTION
    mat4 projection_mat = perspective(radians(m_fovy), (float)width/(float)height, m_z_near, m_z_far);
    glUniformMatrix4fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::PROJ_MAT_LOC_NAME), 1,
                       GL_FALSE, &projection_mat[0][0]);
}

void Scene::adapt_viewport() {
    int width,height;
    glfwGetWindowSize(m_window, &width, &height);
    glViewport(0, 0, width, height);
}
