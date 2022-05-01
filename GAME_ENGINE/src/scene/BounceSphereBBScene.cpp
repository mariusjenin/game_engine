//
// Created by mariusjenin on 07/04/2022.
//

#include <src/physics/force/GravityForce.hpp>
#include "BounceSphereBBScene.hpp"

using namespace scene;

BounceSphereBBScene::BounceSphereBBScene(const std::string &vertex_shader_path, const std::string &fragment_shader_path) : Scene(
        vertex_shader_path, fragment_shader_path) {

    //Physics System
    m_physics_system = new PhysicsSystem(0.005f,0.005f, 5);

    //BACKGROUND
    glClearColor(0.15f, 0.15f, 0.15f, 0.0f);

    //MESHES
    auto *ball_mesh1 = new Mesh(create_sphere(1, 60, 60),true,SphereBB_TYPE);
    auto *ball_mesh2 = new Mesh(create_sphere(5, 120, 120), true,SphereBB_TYPE);
//    auto *ball_mesh1 = new LODMesh(create_sphere(1, 60, 60),2, 30, 60, 5, 10,SphereBB_TYPE);
//    auto *ball_mesh2 = new LODMesh(create_sphere(5, 120, 120),  2, 30, 60, 5, 10,SphereBB_TYPE);
//    auto *plane_mesh1 = new LODMesh(create_plane(100, 100, {-10, 0, -10}, {10, 0, 10}, Y_NORMAL_DIRECTION), 2, 30, 60, 5, 10,AABB_TYPE);
//    auto *plane_mesh2 = new LODMesh(create_plane(100, 100, {-10, 0, -10}, {10, 0, 10}, Y_INV_NORMAL_DIRECTION), 2, 30, 60, 5, 10,AABB_TYPE);


    //Light
    auto *light_source = new DirectionLight({0.1, 0.1, 0.1}, {1., 1., 1.}, {0.8, 0.8, 0.8}, {0., -1., -0.5});
    auto *light_node = new NodeGameSG(m_shaders, m_root);
    light_node->set_light(light_source);
    m_lights.push_back(light_node);

    //Big Ball
    auto* big_ball_mat_color = new MaterialColor(m_shaders, {0.15, 0.55, 0.7}, 50);
    auto* big_ball = new NodeGameSG(m_shaders, m_root,SphereBB_TYPE);
    float gap = 4;
    big_ball->get_trsf()->set_translation({-gap,0,-gap});
    big_ball->set_meshes({ball_mesh2});
    big_ball->set_material(big_ball_mat_color);
    auto* big_ball2 = new NodeGameSG(m_shaders, m_root,SphereBB_TYPE);
    big_ball2->get_trsf()->set_translation({-gap,0,gap});
    big_ball2->set_meshes({ball_mesh2});
    big_ball2->set_material(big_ball_mat_color);
    auto* big_ball3 = new NodeGameSG(m_shaders, m_root,SphereBB_TYPE);
    big_ball3->get_trsf()->set_translation({gap,0,-gap});
    big_ball3->set_meshes({ball_mesh2});
    big_ball3->set_material(big_ball_mat_color);
    auto* big_ball4 = new NodeGameSG(m_shaders, m_root,SphereBB_TYPE);
    big_ball4->get_trsf()->set_translation({gap,0,gap});
    big_ball4->set_meshes({ball_mesh2});
    big_ball4->set_material(big_ball_mat_color);
    m_physics_system->add_rigid_body(new RigidBodyVolume(big_ball,0,0.01,2));
    m_physics_system->add_rigid_body(new RigidBodyVolume(big_ball2,0,0.01,2));
    m_physics_system->add_rigid_body(new RigidBodyVolume(big_ball3,0,0.01,2));
    m_physics_system->add_rigid_body(new RigidBodyVolume(big_ball4,0,0.01,2));
    big_ball->set_debug_rendering(true,{1,0,0});
    big_ball2->set_debug_rendering(true,{0,1,0});
    big_ball3->set_debug_rendering(true,{0,0,1});
    big_ball4->set_debug_rendering(true,{0,1,1});


    //Ball
    m_ball = new NodeGameSG(m_shaders, m_root,SphereBB_TYPE);
    m_ball->get_trsf()->set_translation({2.,30,2.2});
    m_ball->get_trsf()->set_rotation({25,74,42});
    m_ball->get_trsf()->set_uniform_scale(1/2.f);
    m_ball->set_meshes({ball_mesh1});
    m_ball->set_material(new MaterialColor(m_shaders, {0.75, 0.3, 0.95}, 50));
    m_ball->set_debug_rendering(true,{1,0,1});
    auto* gravity_force = new GravityForce();
    auto* rbv_ball = new RigidBodyVolume(m_ball,1000,0.01,1);
    rbv_ball->add_force(gravity_force);
    m_physics_system->add_rigid_body(rbv_ball);

    //Ball2
    auto* m_ball2 = new NodeGameSG(m_shaders, m_ball,SphereBB_TYPE);
    m_ball2->get_trsf()->set_translation({2,2,2});
    m_ball2->get_trsf()->set_uniform_scale(2.f);
    m_ball2->set_meshes({ball_mesh1});
    m_ball2->set_material(new MaterialColor(m_shaders, {0.85, 0.5, 0.45}, 50));

    //CAMERA
    auto *camera_node = new NodeGameSG(m_shaders, m_root);
    camera_node->get_trsf()->set_translation({0, 17, 28});
    camera_node->get_trsf()->set_rotation({-27, 0, 0});
    m_cameras.push_back(camera_node);

    //PROJECTION
    mat4 projection_mat = perspective(radians(45.0f), 4.f / 3.0f, 0.1f, 10000.0f);
    glUniformMatrix4fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::PROJ_MAT_LOC_NAME), 1,
                       GL_FALSE, &projection_mat[0][0]);
}


void BounceSphereBBScene::process_input(GLFWwindow *window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float camera_speed = 15.f * delta_time;
    float camera_speed_rot = 150 * delta_time;
    float ball_translate_speed = 15 * delta_time;

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

//    Transform *ball_trsf = m_ball->get_trsf();
    glm::vec3 translate_ball;
    bool impulse_ball = false;
    //Scene rotation
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        translate_ball += glm::vec3(ball_translate_speed, 0.f, 0.f);
        impulse_ball =  true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        translate_ball -= glm::vec3(ball_translate_speed, 0.f, 0.f);
        impulse_ball =  true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        translate_ball += glm::vec3(0.f, 0.f, ball_translate_speed);
        impulse_ball =  true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        translate_ball -= glm::vec3(0.f, 0.f, ball_translate_speed);
        impulse_ball =  true;
    }
    if(impulse_ball)m_ball->get_rigid_body()->add_linear_impulse(translate_ball);
//    ball_trsf->set_translation(ball_trsf->get_translation() + translate_ball);
//    if (!ball_trsf->is_up_to_date()) ball_trsf->compute();
}
