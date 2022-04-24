//
// Created by mariusjenin on 05/03/2022.
//

#include <src/material/MaterialColor.hpp>
#include <src/material/MaterialTexture.hpp>
#include <src/light/PositionLight.hpp>
#include <src/mesh/LODMesh.hpp>
#include "SolarSystem.hpp"

using namespace scene;

SolarSystem::SolarSystem(const std::string &vertex_shader_path, const std::string &fragment_shader_path) : Scene(
        vertex_shader_path, fragment_shader_path) {
    GLuint program_id = m_shaders->get_program_id();
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();

    m_timing = 0;

    int id_texture = 0;
    //TEXTURE HEIGHT MAP
    shader_data_manager->load_custom_uniform_location(program_id, HM_SUN_LOC_NAME);
    shader_data_manager->load_custom_uniform_location(program_id, HM_EARTH_LOC_NAME);
    shader_data_manager->load_custom_uniform_location(program_id, HM_MOON_LOC_NAME);
    glUniform1i(shader_data_manager->get_location(HM_SUN_LOC_NAME), id_texture);
    load_bmp_custom("../assets/height_map/hm_sun.bmp", id_texture++);
    glUniform1i(shader_data_manager->get_location(HM_EARTH_LOC_NAME), id_texture);
    load_bmp_custom("../assets/height_map/hm_earth.bmp", id_texture++);
    glUniform1i(shader_data_manager->get_location(HM_MOON_LOC_NAME), id_texture);
    load_bmp_custom("../assets/height_map/hm_moon.bmp", id_texture++);

    //UNIFORM DEFINED VALUES
    load_type_star_location();

    //UNIFORM VALUES
    shader_data_manager->load_custom_uniform_location(program_id, ALWAYS_ENLIGHTENED_LOC_NAME);
    GLint always_enlightened_location = shader_data_manager->get_location(ALWAYS_ENLIGHTENED_LOC_NAME);
    shader_data_manager->load_custom_uniform_location(program_id, STAR_ID_LOC_NAME);
    GLint star_id_location = shader_data_manager->get_location(STAR_ID_LOC_NAME);

    //BACKGROUND
    glClearColor(0.04f, 0.f, 0.13f, 0.0f);

    //VALUES
    float tilt_earth = 23.44f;
    float tilt_moon = 6.68f;
    float inclination_sun_earth = 5.14f;

    //MESHES
    auto *sky_mesh = new Mesh(create_sphere(100, 10, 20));
    auto *sun_mesh = new LODMesh(create_sphere(2, 50, 100), 20, 60, 12, 2, 5);
    auto *earth_mesh = new LODMesh(create_sphere(1, 30, 60), 20, 60, 12, 2, 5);
    auto *moon_mesh = new LODMesh(create_sphere(0.25, 20, 40), 20, 60, 12, 2, 5);

    //TEXTURES
    int id_skymap_texture = id_texture++;
    int id_sun_texture = id_texture++;
    int id_earth_texture = id_texture++;
    int id_moon_texture = id_texture;
    load_bmp_custom("../assets/texture/skymap.bmp", id_skymap_texture);
    load_bmp_custom("../assets/texture/sun.bmp", id_sun_texture);
    load_bmp_custom("../assets/texture/earth.bmp", id_earth_texture);
    load_bmp_custom("../assets/texture/moon.bmp", id_moon_texture);

    //SKY
    m_sky = new NodeSG(m_shaders, (ElementSG *) m_root);
    m_sky->get_local_trsf()->set_rotation({-90, 0, 0});
    m_sky->set_meshes({sky_mesh});
    m_sky->set_material(new MaterialTexture(m_shaders, id_skymap_texture));
    m_sky->add_uniform_1i(star_id_location, NOT_STAR_ID);
    m_sky->add_uniform_1i(always_enlightened_location, true);
    m_sky->set_see_both_face(true);
    m_nodes.push_back(m_sky);

    //SUN
    auto *sun_light = new PositionLight({0.1, 0.1, 0.1}, {1., 1., 1.}, {0.5, 0.5, 0.5}, 1.0, 0.007, 0.0002);
    m_sun = new LightNodeSG(m_shaders, (ElementSG *) m_sky, sun_light, SUN_NAME);
    m_sun->get_local_trsf()->set_rotation({-90, 0, 0});
    m_sun->set_meshes({sun_mesh});
    m_sun->set_material(new MaterialTexture(m_shaders, id_sun_texture));
    m_sun->add_uniform_1i(star_id_location, SUN_ID);
    m_sun->add_uniform_1i(always_enlightened_location, true);
    m_nodes.push_back(m_sun);
    m_lights.push_back(m_sun);

    //EARTH
    m_earth1 = new NodeSG(m_shaders, (ElementSG *) m_sun);
    m_earth2 = new NodeSG(m_shaders, (ElementSG *) m_earth1);
    m_earth3 = new NodeSG(m_shaders, (ElementSG *) m_earth2, EARTH_NAME);
    m_earth3->get_local_trsf()->set_rotation({-90, 180, -tilt_earth});
    m_earth3->get_local_trsf()->set_order_rotation(ORDER_ZYX);
    m_earth1->get_trsf()->set_rotation({0, 0, -inclination_sun_earth});
    m_earth3->get_trsf()->set_translation({10, 0, 0});
    m_earth3->set_meshes({earth_mesh});
//    m_earth->set_material(new MaterialColor(m_shaders,{0.8,0.2,0.4},15));
    m_earth3->set_material(new MaterialTexture(m_shaders, id_earth_texture));
    m_earth3->add_uniform_1i(star_id_location, EARTH_ID);
    m_earth3->add_uniform_1i(always_enlightened_location, false);
    m_nodes.push_back(m_earth1);
    m_nodes.push_back(m_earth2);
    m_nodes.push_back(m_earth3);

    //MOON
    m_moon1 = new NodeSG(m_shaders, (ElementSG *) m_earth3, MOON_NAME);
    m_moon2 = new NodeSG(m_shaders, (ElementSG *) m_moon1, MOON_NAME);
    m_moon3 = new NodeSG(m_shaders, (ElementSG *) m_moon2, MOON_NAME);
    m_moon3->get_local_trsf()->set_rotation({-90, 0, tilt_moon});
    m_moon3->get_local_trsf()->set_order_rotation(ORDER_ZYX);
//    m_moon->set_trsfs_general({moon_trsf_1, moon_trsf_2, moon_trsf_3});
    m_moon1->get_trsf()->set_rotation({0, 0, inclination_sun_earth});
    m_moon3->get_trsf()->set_translation({3, 0, 0});
    m_moon3->set_meshes({moon_mesh});
//    m_moon->set_material(new MaterialColor(m_shaders,{0.4,0.2,0.8},15));
    m_moon3->set_material(new MaterialTexture(m_shaders, id_moon_texture));
    m_moon3->add_uniform_1i(star_id_location, MOON_ID);
    m_moon3->add_uniform_1i(always_enlightened_location, false);
    m_nodes.push_back(m_moon1);
    m_nodes.push_back(m_moon2);
    m_nodes.push_back(m_moon3);

    //CAMERA
    auto *camera_node = new CameraNodeSG(m_shaders,
                                         (ElementSG *) m_root);
    camera_node->get_trsf()->set_translation({0, 5, 20});
    camera_node->get_trsf()->set_rotation({-15, 0, 0});
    m_cameras.push_back(camera_node);

    //CAMERA 2
    auto *camera_node_2 = new CameraNodeSG(m_shaders,
                                           (ElementSG *) m_earth3);

    camera_node_2->get_trsf()->set_rotation({0, -90, 0});
    m_cameras.push_back(camera_node_2);

    m_camera_index = 0;

    //PROJECTION
    mat4 projection_mat = perspective(radians(45.0f), 4.f / 3.0f, 0.1f, 100000000.0f);
    glUniformMatrix4fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::PROJ_MAT_LOC_NAME), 1,
                       GL_FALSE, &projection_mat[0][0]);

    //SPEED ANIMATION
    m_speed_anime = 5.f;
}

void SolarSystem::update(GLFWwindow *window, float delta_time) {

    float speed_rotation_sky = 0.2 * delta_time * m_speed_anime;
    float speed_rotation_sun = 10 * delta_time * m_speed_anime;
    float speed_rotation_earth = 100 * delta_time * m_speed_anime;
    float speed_rotation_moon = 50 * delta_time * m_speed_anime;

    float speed_revolution_earth = 10 * delta_time * m_speed_anime;
    float speed_revolution_moon = speed_rotation_moon;

    CameraNodeSG *camera_node = m_cameras[m_camera_index];

    //ROTATION SKY
    glm::vec3 rotation_sky = glm::vec3(0.f, speed_rotation_sky, speed_rotation_sky);
    Transform *sky_trsf_intern_before = ((NodeSG *) m_sky)->get_local_trsf();
    sky_trsf_intern_before->set_rotation(sky_trsf_intern_before->get_rotation() + rotation_sky);
    sky_trsf_intern_before->compute();

    //ROTATION SUN
    glm::vec3 rotation_sun = glm::vec3(0.f, speed_rotation_sun, 0.f);
    Transform *sun_trsf_intern_before = ((NodeSG *) m_sun)->get_local_trsf();
    sun_trsf_intern_before->set_rotation(sun_trsf_intern_before->get_rotation() + rotation_sun);
    sun_trsf_intern_before->compute();

    //ROTATION EARTH
    glm::vec3 rotation_earth = glm::vec3(0.f, speed_rotation_earth, 0.f);
    Transform *earth_trsf_intern_before = ((NodeSG *) m_earth3)->get_local_trsf();
    earth_trsf_intern_before->set_rotation(earth_trsf_intern_before->get_rotation() + rotation_earth);
    earth_trsf_intern_before->compute();

    //ROTATION MOON
    glm::vec3 rotation_moon = glm::vec3(0.f, speed_rotation_moon, 0.f);
    Transform *moon_trsf_intern_before = ((NodeSG *) m_moon3)->get_local_trsf();
    moon_trsf_intern_before->set_rotation(moon_trsf_intern_before->get_rotation() + rotation_moon);
    moon_trsf_intern_before->compute();

    //REVOLUTION EARTH & "PERMANENT TILT" EARTH
    glm::vec3 revolution_earth = glm::vec3(0.f, speed_revolution_earth, 0.f);
    Transform *earth_trsf_1 = ((NodeSG *) m_earth2)->get_trsf();
    Transform *earth_trsf_2 = ((NodeSG *) m_earth3)->get_trsf();
    earth_trsf_1->set_rotation(earth_trsf_1->get_rotation() + revolution_earth);
    earth_trsf_2->set_rotation(earth_trsf_2->get_rotation() - revolution_earth);
    earth_trsf_1->compute();
    earth_trsf_2->compute();

    //REVOLUTION MOON & "PERMANENT TILT" MOON
    //We could remove the permanent tilt of the moon and the rotation of the moon because they cancel themselves (same speed)
    glm::vec3 revolution_moon = glm::vec3(0.f, speed_revolution_moon, 0.f);
    Transform *moon_trsf_1 = ((NodeSG *) m_moon2)->get_trsf();
    Transform *moon_trsf_2 = ((NodeSG *) m_moon3)->get_trsf();
    moon_trsf_1->set_rotation(moon_trsf_1->get_rotation() + revolution_moon);
    moon_trsf_2->set_rotation(moon_trsf_2->get_rotation() - revolution_moon);
    moon_trsf_1->compute();
    moon_trsf_2->compute();


    //ROTATION CAMERA if camera set on a star
    if (m_camera_index == 1) {
        glm::vec3 rotation_camera = glm::vec3(0.f, speed_revolution_moon, 0.f);
        Transform *camera_trsf_self_before = camera_node->get_local_trsf();
        camera_trsf_self_before->set_rotation(camera_trsf_self_before->get_rotation() + rotation_camera);
        camera_trsf_self_before->compute();
    }

    Scene::update(window, delta_time);
}


void SolarSystem::process_input(GLFWwindow *window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float camera_speed = 7.f * delta_time;
    float camera_speed_rot = 100 * delta_time;
    float model_speed_rot = 100 * delta_time;

    m_timing -= delta_time;

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && m_timing <= 0) {
        m_camera_index = (m_camera_index + 1) % 2;
        m_timing = 0.5;
    }

    //Speed animations
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        m_speed_anime = min(100.f, m_speed_anime + 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        m_speed_anime = max(0.f, m_speed_anime - 0.1f);
    }

    Transform *camera_trsf = m_cameras.at(m_camera_index)->get_local_trsf();
    bool camera_changed = false;
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

    Transform *scene_trsf = m_sky->get_trsf();
    glm::vec3 rot_scene;
    //Scene rotation
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        rot_scene += glm::vec3(0.f, 0.f, model_speed_rot);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        rot_scene -= glm::vec3(0.f, 0.f, model_speed_rot);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        rot_scene += glm::vec3(model_speed_rot, 0.f, 0.f);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
        rot_scene -= glm::vec3(model_speed_rot, 0.f, 0.f);
    }
    scene_trsf->set_rotation(scene_trsf->get_rotation() + rot_scene);
    if (!scene_trsf->is_up_to_date()) scene_trsf->compute();

}

void SolarSystem::load_type_star_location() const {
    GLuint program_id = m_shaders->get_program_id();
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
    shader_data_manager->load_custom_uniform_location(program_id, SUN_LOC_NAME);
    shader_data_manager->load_custom_uniform_location(program_id, EARTH_LOC_NAME);
    shader_data_manager->load_custom_uniform_location(program_id, MOON_LOC_NAME);
    glUniform1i(shader_data_manager->get_location(SUN_LOC_NAME), SUN_ID);
    glUniform1i(shader_data_manager->get_location(EARTH_LOC_NAME), EARTH_ID);
    glUniform1i(shader_data_manager->get_location(MOON_LOC_NAME), MOON_ID);
}


