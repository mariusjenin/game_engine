//
// Created by mariusjenin on 19/03/2022.
//


#include <src/light/PositionLight.hpp>
#include <src/scene_graph/NodeOnTopSG.hpp>
#include <src/light/SpotLight.hpp>
#include "SceneLand.hpp"

using namespace scene;

SceneLand::SceneLand(const std::string &vertex_shader_path, const std::string &fragment_shader_path) : Scene(
        vertex_shader_path, fragment_shader_path) {
    GLuint program_id = m_shaders->get_program_id();
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();

    //BACKGROUND
    glClearColor(0.4f, 0.7f, 0.9f, 0.0f);

    //TEXTURES
    int id_texture = 0;
    int id_land_texture = id_texture++;
    load_bmp_custom("../assets/texture/rock.bmp", id_land_texture);


    //TEXTURE HEIGHT MAP
    shader_data_manager->load_custom_uniform_location(program_id, HM_LAND_LOC_NAME);
    glUniform1i(shader_data_manager->get_location(HM_LAND_LOC_NAME), id_texture);
    load_bmp_custom("../assets/height_map/hm_land.bmp", id_texture);

    //UNIFORM VALUES
    shader_data_manager->load_custom_uniform_location(program_id, HAS_HM_LOC_NAME);
    GLint has_hm_location = shader_data_manager->get_location(HAS_HM_LOC_NAME);

    //MESHES
    auto *land_mesh = create_plane(100, 100, {-10, 0, -10}, {10, 0, 10}, {0, 1, 0});
    auto *ball_mesh = create_sphere(1, 60, 60);
//    auto *land_mesh = new LODMesh(*create_plane(100, 100, {-10, 0, -10}, {10, 0, 10}, {0, 1, 0}), 0, 200, 10, 95, 8);
//    auto *ball_mesh = new LODMesh(*create_sphere(1, 60, 60), 10, 50, 5, 60, 6);

    //Land
    auto *land_node = new NodeSG(m_shaders, m_root, "LAND");
    land_node->set_material(new MaterialTexture(m_shaders, id_land_texture));
    land_node->set_trsfs_self_before({new Transform({0, 0, 0}, {0, 0, 0}, {5, 5, 5})});
    land_node->set_meshes({land_mesh});
    land_node->set_see_both_face(true);
    land_node->add_uniform_1i(has_hm_location, true);
    m_nodes.push_back(land_node);

    //Light
    auto *light_source = new DirectionLight({0.1, 0.1, 0.1}, {1., 1., 1.}, {0.8, 0.8, 0.8}, {0., -1., 0.});
    auto *light_node = new LightNodeSG(m_shaders, m_root, light_source);
    m_nodes.push_back(light_node);
    m_lights.push_back(light_node);

    //Ball
    m_ball = new NodeOnTopSG(m_shaders, land_node, land_node);
    m_ball->set_trsfs_general({new Transform({-8, 0, 10})});
    m_ball->set_meshes({ball_mesh});
    m_ball->set_material(
            new MaterialColor(m_shaders, {0.75, 0.3, 0.95}, 50));
    m_ball->add_uniform_1i(has_hm_location, false);
    m_nodes.push_back(m_ball);

    //CAMERA
    auto *camera_node = new CameraNodeSG(m_shaders, m_root);
    camera_node->set_trsfs_general({new Transform({-7, -10, 40}, {-5, 0, 0})});
    m_cameras.push_back(camera_node);

    //PROJECTION
    mat4 projection_mat = perspective(radians(45.0f), 4.f / 3.0f, 0.1f, 10000.0f);
    glUniformMatrix4fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::PROJ_MAT_LOC_NAME), 1,
                       GL_FALSE, &projection_mat[0][0]);
}


void SceneLand::process_input(GLFWwindow *window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float camera_speed = 15.f * delta_time;
    float camera_speed_rot = 150 * delta_time;
    float ball_translate_speed = 15 * delta_time;

    Transform *camera_trsf = m_cameras.at(m_camera_index)->get_trsfs_general()[0];
    //Camera Translation
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        glm::vec3 dir;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            dir += glm::vec3(camera_speed, 0.f, 0.f);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            dir += glm::vec3(-camera_speed, 0.f, 0.f);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            dir += glm::vec3(0.f, camera_speed, 0.f);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            dir += glm::vec3(0.f, -camera_speed, 0.f);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            dir += glm::vec3(0.f, 0.f, +camera_speed);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            dir += glm::vec3(0.f, 0.f, -camera_speed);
        }
        camera_trsf->set_translation(camera_trsf->get_translation() + camera_trsf->apply_to_vector(dir));
    }

    //Camera rotation
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        glm::vec3 rot;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            rot += glm::vec3(camera_speed_rot, 0.f, 0.f);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            rot -= glm::vec3(camera_speed_rot, 0.f, 0.f);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            rot += glm::vec3(0.f, camera_speed_rot, 0.f);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            rot -= glm::vec3(0.f, camera_speed_rot, 0.f);
        }
        camera_trsf->set_rotation(camera_trsf->get_rotation() + rot);
    }
    if (!camera_trsf->is_up_to_date()) camera_trsf->compute();

    Transform *ball_trsf = m_ball->get_trsfs_general()[0];
    glm::vec3 translate_ball;
    //Scene rotation
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        translate_ball += glm::vec3(ball_translate_speed, 0.f, 0.f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        translate_ball -= glm::vec3(ball_translate_speed, 0.f, 0.f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        translate_ball += glm::vec3(0.f, 0.f, ball_translate_speed);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        translate_ball -= glm::vec3(0.f, 0.f, ball_translate_speed);
    }
    ball_trsf->set_translation(ball_trsf->get_translation() + translate_ball);
    if (!ball_trsf->is_up_to_date()) ball_trsf->compute();
}

SceneLand::~SceneLand() {
    delete m_ball;
}

