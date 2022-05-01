//
// Created by mariusjenin on 07/04/2022.
//

#include <src/physics/force/GravityForce.hpp>
#include "BounceOBBScene.hpp"

using namespace scene;

BounceOBBScene::BounceOBBScene(const std::string &vertex_shader_path, const std::string &fragment_shader_path) : Scene(
        vertex_shader_path, fragment_shader_path) {

    //BACKGROUND
    glClearColor(0.15f, 0.15f, 0.15f, 0.0f);

    //Physics System
    m_physics_system = new PhysicsSystem(0.2f, 0.01f, 5);

    //MESHES
    auto *cube_mesh = new Mesh(create_rectangle_cuboid({5,5,5}), true,OBB_TYPE);
    auto *ball_mesh1 = new Mesh(create_sphere(1, 60, 60),true,SphereBB_TYPE);
    auto *cube_mesh2 = new Mesh(create_rectangle_cuboid({1,1,1}), true,OBB_TYPE);

    //Light
    auto *light_source = new DirectionLight({0.1, 0.1, 0.1}, {1., 1., 1.}, {0.8, 0.8, 0.8}, {-0.2, -1., -0.5});
    auto *light_node = new NodeGameSG(m_shaders, m_root);
    light_node->set_light(light_source);
    m_lights.push_back(light_node);

    //Big cube
    auto* big_cube_mat_color = new MaterialColor(m_shaders, {0.15, 0.55, 0.7}, 50);
    auto* big_cube = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    big_cube->get_trsf()->set_translation({0,0,0});
    big_cube->get_trsf()->set_rotation({75, -90, 0});
    big_cube->set_meshes({cube_mesh});
    big_cube->set_material(big_cube_mat_color);
//    big_cube->set_debug_rendering(true, {0.25, 0.65, 0.8});
    m_physics_system->add_rigid_body(new RigidBodyVolume(big_cube,0,0.01,2));


    //cube
    m_cube = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    m_cube->get_trsf()->set_translation({0,20,0});
     m_cube->get_trsf()->set_rotation({88,10,33});
    m_cube->get_trsf()->set_uniform_scale(1/2.f);
    m_cube->set_meshes({cube_mesh2});
    m_cube->set_material(new MaterialColor(m_shaders, {0.75, 0.3, 0.95}, 50));
//    m_cube->set_debug_rendering(true, {0.85, 0.5, 1});
    auto* gravity_force = new GravityForce();
    auto* rbv_cube = new RigidBodyVolume(m_cube,1,0.01,0.6);
    rbv_cube->add_force(gravity_force);
    m_physics_system->add_rigid_body(rbv_cube);


    //ball
    auto* m_ball = new NodeGameSG(m_shaders, m_root,SphereBB_TYPE);
    m_ball->get_trsf()->set_translation({0.,20,0});
    m_ball->set_meshes({ball_mesh1});
    m_ball->set_material(new MaterialColor(m_shaders, {0.75, 0.3, 0.95}, 50));
//    m_ball->set_debug_rendering(true);
    auto* rbv_sphere = new RigidBodyVolume(m_ball,1,0.01,1);
    rbv_sphere->add_force(gravity_force);
    m_physics_system->add_rigid_body(rbv_sphere);

//   Cube 2
    auto* m_cube2 = new NodeGameSG(m_shaders, m_cube,OBB_TYPE);
    m_cube2->get_trsf()->set_translation({2,2,2});
//    m_cube2->get_trsf()->set_uniform_scale(1/5.f);
    m_cube2->set_meshes({cube_mesh2});
    m_cube2->set_material(new MaterialColor(m_shaders, {0.85, 0.5, 0.45}, 50));


    //CAMERA
    auto *camera_node = new NodeGameSG(m_shaders, m_root);
    camera_node->get_trsf()->set_translation({0, 9, 17});
    camera_node->get_trsf()->set_rotation({-20, 0, 0});
    m_cameras.push_back(camera_node);

    //PROJECTION
    mat4 projection_mat = perspective(radians(45.0f), 4.f / 3.0f, 0.1f, 10000.0f);
    glUniformMatrix4fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::PROJ_MAT_LOC_NAME), 1,
                       GL_FALSE, &projection_mat[0][0]);
}


void BounceOBBScene::process_input(GLFWwindow *window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float camera_speed = 15.f * delta_time;
    float camera_speed_rot = 150 * delta_time;
    float cube_translate_speed = 15 * delta_time;

    Transform *camera_trsf = m_cameras.at(m_camera_index)->get_trsf();
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

//    Transform *cube_trsf = m_cube->get_trsf();
    glm::vec3 translate_cube;
    bool impulse_cube = false;
    //Scene rotation
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        translate_cube += glm::vec3(cube_translate_speed, 0.f, 0.f);
        impulse_cube =  true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        translate_cube -= glm::vec3(cube_translate_speed, 0.f, 0.f);
        impulse_cube =  true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        translate_cube += glm::vec3(0.f, 0.f, cube_translate_speed);
        impulse_cube =  true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        translate_cube -= glm::vec3(0.f, 0.f, cube_translate_speed);
        impulse_cube =  true;
    }
    if(impulse_cube)m_cube->get_rigid_body()->add_linear_impulse(translate_cube);
//    cube_trsf->set_translation(cube_trsf->get_translation() + translate_cube);
//    if (!cube_trsf->is_up_to_date()) cube_trsf->compute();
}
