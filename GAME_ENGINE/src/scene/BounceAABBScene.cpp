//
// Created by mariusjenin on 07/04/2022.
//

#include <src/physics/force/GravityForce.hpp>
#include <src/physics/rigid_body_behavior/MovementBehavior.hpp>
#include "BounceAABBScene.hpp"

using namespace scene;

BounceAABBScene::BounceAABBScene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path, float mult_physics) : Scene(window,
        vertex_shader_path, fragment_shader_path, mult_physics) {

    //BACKGROUND
    glClearColor(0.15f, 0.15f, 0.15f, 0.0f);

    //Physics System
    m_physics_system = new PhysicsSystem(m_root,mult_physics,0.9f, 0.05f, 15,EULER_TYPE);

    //MESHES
    auto *cube_mesh = new Mesh(create_rectangle_cuboid({5,5,5}), true,AABB_TYPE);
    auto *cube_mesh2 = new Mesh(create_rectangle_cuboid({1,1,1}), true,AABB_TYPE);

    //Light
    auto *light_source = new DirectionLight({0.1, 0.1, 0.1}, {1., 1., 1.}, {0.8, 0.8, 0.8});
    auto *light_node = new NodeGameSG(m_root);
    light_node->get_trsf()->set_rotation({-90,0,0});
    light_node->set_light(light_source);
    m_lights.push_back(light_node);

    //Big cube
    auto* big_cube_mat_color = new MaterialColor({0.15, 0.55, 0.7}, 50);
    auto* big_cube = new NodeGameSG(m_root,AABB_TYPE);
    big_cube->get_trsf()->set_translation({0,0,0});
    big_cube->set_meshes({cube_mesh});
    big_cube->set_material(big_cube_mat_color);
    big_cube->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto * rbv_big_cube = new RigidBodyVolume(big_cube);
    rbv_big_cube->add_behavior(new MovementBehavior(false,false,mult_physics,0,0.01,2));
    m_physics_system->add_collider(rbv_big_cube);


    //cube
    m_cube = new NodeGameSG(m_root,AABB_TYPE);
    m_cube->get_trsf()->set_translation({0,10,0});
    m_cube->get_trsf()->set_rotation({0,0,35});
    m_cube->get_trsf()->set_uniform_scale(2.f);
    m_cube->set_meshes({cube_mesh2});
    m_cube->set_material(new MaterialColor({0.75, 0.3, 0.95}, 50));
    m_cube->set_debug_rendering(true, {0.85, 0.5, 1});
    auto* gravity_force = new GravityForce();
    auto* rbv_cube = new RigidBodyVolume(m_cube);
    rbv_cube->add_behavior(new MovementBehavior(true,true,mult_physics,0.1,0.01,0.4));
    rbv_cube->get_movement_behavior()->add_force(gravity_force);
    m_physics_system->add_collider(rbv_cube);


    //CAMERA
    auto *camera_node = new NodeGameSG(m_root);
    camera_node->get_trsf()->set_translation({0, 9, 17});
    camera_node->get_trsf()->set_rotation({-20, 0, 0});
    m_cameras.push_back(camera_node);


    //PROJECTION
    m_fovy = 45.0f;
    m_z_near = 0.1f;
    m_z_far = 100.0f;
}


void BounceAABBScene::process_input(float delta_time) {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
    float camera_speed = 15.f * delta_time;
    float camera_speed_rot = 150 * delta_time;
    float cube_translate_speed = 15 * delta_time;

    Transform *camera_trsf = m_cameras.at(m_camera_index)->get_trsf();
    //Camera Translation
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        glm::vec3 dir;
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            dir += glm::vec3(camera_speed, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            dir += glm::vec3(-camera_speed, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) {
            dir += glm::vec3(0.f, camera_speed, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
            dir += glm::vec3(0.f, -camera_speed, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            dir += glm::vec3(0.f, 0.f, +camera_speed);
        }
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            dir += glm::vec3(0.f, 0.f, -camera_speed);
        }
        camera_trsf->set_translation(camera_trsf->get_translation() + camera_trsf->apply_to_vector(dir));
    }

    //Camera rotation
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        glm::vec3 rot;
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            rot += glm::vec3(camera_speed_rot, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            rot -= glm::vec3(camera_speed_rot, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            rot += glm::vec3(0.f, camera_speed_rot, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            rot -= glm::vec3(0.f, camera_speed_rot, 0.f);
        }
        camera_trsf->set_rotation(camera_trsf->get_rotation() + rot);
    }
    if (!camera_trsf->is_up_to_date()) camera_trsf->compute();

//    Transform *cube_trsf = m_cube->get_trsf();
    glm::vec3 translate_cube;
    bool impulse_cube = false;
    //Scene rotation
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        translate_cube += glm::vec3(cube_translate_speed, 0.f, 0.f);
        impulse_cube =  true;
    }
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        translate_cube -= glm::vec3(cube_translate_speed, 0.f, 0.f);
        impulse_cube =  true;
    }
    if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        translate_cube += glm::vec3(0.f, 0.f, cube_translate_speed);
        impulse_cube =  true;
    }
    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
        translate_cube -= glm::vec3(0.f, 0.f, cube_translate_speed);
        impulse_cube =  true;
    }
    if(impulse_cube)m_cube->get_rigid_body()->get_movement_behavior()->add_linear_impulse(translate_cube);
}
