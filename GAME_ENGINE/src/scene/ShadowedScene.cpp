//
// Created by mariusjenin on 09/05/2022.
//

#include "ShadowedScene.hpp"
#include <src/physics/force/GravityForce.hpp>
#include <src/physics/rigid_body_behavior/MovementBehavior.hpp>

using namespace scene;

ShadowedScene::ShadowedScene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path, float mult_physics) : Scene(window,
        vertex_shader_path, fragment_shader_path, mult_physics) {

    TextureManager *texture_manager = m_shaders->get_texture_manager();
    //BACKGROUND
    glClearColor(0.f, 0.15f, 0.3f, 0.0f);

    //Physics System
    m_physics_system = new PhysicsSystem(m_root,mult_physics,0.2f, 0.f, 15,EULER_TYPE);

    //MESHES
    auto *floor_mesh = new Mesh(create_rectangle_cuboid({20,0.5,20}), true,AABB_TYPE);
    auto *cube_mesh = new Mesh(create_rectangle_cuboid({2,8,2}), true,AABB_TYPE);
    auto *ball_mesh = new Mesh(create_sphere(1, 30, 30), true, SPHEREBB_TYPE);

    //Light
    auto *light_source = new SpotLight({0.2, 0.2, 0.2}, {1., 1., 1.}, {0.8, 0.8, 0.8},texture_manager->get_and_increment_id_texture(),15,25);
    m_sphere_light = new NodeGameSG(m_root,SPHEREBB_TYPE);
//    m_sphere_light->get_trsf()->set_translation({0,25,0});
    m_sphere_light->get_trsf()->set_translation({0,5,20});
//    m_sphere_light->get_trsf()->set_rotation({-90,0.f,0});
    m_sphere_light->get_trsf()->set_order_rotation(ORDER_ZXY);
    m_sphere_light->set_light(light_source);
    m_sphere_light->set_meshes({ball_mesh});
    m_sphere_light->set_material(new MaterialColor({1., 1., 0.8}, 50));
    m_sphere_light->set_debug_rendering(true, {0.7, 0.7, 0.4});
    m_lights.push_back(m_sphere_light);

    auto *light_source2 = new SpotLight({0.2, 0.2, 0.2}, {1., 1., 1.}, {0.8, 0.8, 0.8},texture_manager->get_and_increment_id_texture(),15,25);
    auto* sphere_light_2 = new NodeGameSG(m_root,SPHEREBB_TYPE);
    sphere_light_2->get_trsf()->set_translation({15,15,-15});
    sphere_light_2->get_trsf()->set_rotation({-135,35.f,0});
    sphere_light_2->get_trsf()->set_order_rotation(ORDER_ZXY);
    sphere_light_2->set_light(light_source2);
    sphere_light_2->set_meshes({ball_mesh});
    sphere_light_2->set_material(new MaterialColor({1., 1., 0.8}, 50));
    sphere_light_2->set_debug_rendering(true, {0.7, 0.7, 0.4});
    m_lights.push_back(sphere_light_2);

    //Plane
    auto* floor_color = new MaterialColor({0.3, 0.75, 0.2}, 100);
    auto* floor = new NodeGameSG(m_root,AABB_TYPE);
    floor->set_meshes({floor_mesh});
    floor->set_material(floor_color);
//    floor->set_debug_rendering(true, {0.3, 1, 0.1});
    auto * rbv_floor = new RigidBodyVolume(floor);
    rbv_floor->add_behavior(new MovementBehavior(false,false, mult_physics,0,0.5,0.2));
    m_physics_system->add_collider(rbv_floor);

    auto* gravity_force = new GravityForce();

    //cube
    auto* cube = new NodeGameSG(m_root,OBB_TYPE);
    cube->get_trsf()->set_translation({-2,4,-2});
    cube->set_meshes({cube_mesh});
    cube->set_material(new MaterialColor({0.9, 0.3, 0.2}, 100));
//    cube->set_debug_rendering(true, {1., 0.4, 0.3});
    auto* rbv_cube = new RigidBodyVolume(cube);
    rbv_cube->add_behavior(new MovementBehavior(true,true, mult_physics,1,0.3,0.5));
    rbv_cube->get_movement_behavior()->add_force(gravity_force);
    m_physics_system->add_collider(rbv_cube);

    //Sphere
    auto* sphere = new NodeGameSG(m_root,SPHEREBB_TYPE);
    sphere->get_trsf()->set_translation({2,5,3});
    sphere->set_meshes({ball_mesh});
    sphere->set_material(new MaterialColor({0.2, 0.3, 0.9}, 100));
//    sphere->set_debug_rendering(true, {0.3, 0.4, 1.});
    auto* rbv_sphere = new RigidBodyVolume(sphere);
    rbv_sphere->add_behavior(new MovementBehavior(true,true, mult_physics,1,0.3,0.5));
    rbv_sphere->get_movement_behavior()->add_force(gravity_force);
    m_physics_system->add_collider(rbv_sphere);

    //CAMERA
    auto *camera_node = new NodeGameSG(m_root);
//    auto *camera_node = new NodeGameSG(m_sphere_light);
    camera_node->get_trsf()->set_translation({-6, 9, 20});
    camera_node->get_trsf()->set_rotation({-23, -15, 0});
    m_cameras.push_back(camera_node);

    //PROJECTION
    m_fovy = 45.0f;
    m_z_near = 0.1f;
    m_z_far = 100.0f;
}


void ShadowedScene::process_input(float delta_time) {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
    float camera_speed = 35.f * delta_time;
    float camera_speed_rot = 250 * delta_time;
    float sphere_translate_speed = 35 * delta_time;
    float sphere_rotate_speed = 150 * delta_time;

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

    Transform *sphere_trsf = m_sphere_light->get_trsf();
    glm::vec3 translate_sphere;
    //Scene rotation
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
            translate_sphere += glm::vec3(0.f, sphere_translate_speed, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            translate_sphere -= glm::vec3(0.f, sphere_translate_speed, 0.f);
        }
    } else {
        if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            translate_sphere += glm::vec3(sphere_translate_speed, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            translate_sphere -= glm::vec3(sphere_translate_speed, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            translate_sphere += glm::vec3(0.f, 0.f, sphere_translate_speed);
        }
        if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
            translate_sphere -= glm::vec3(0.f, 0.f, sphere_translate_speed);
        }
    }

    glm::vec3 rotate_sphere;
    if (glfwGetKey(m_window, GLFW_KEY_KP_8) == GLFW_PRESS) {
        rotate_sphere += glm::vec3(sphere_rotate_speed, 0.f, 0.f);
    }
    if (glfwGetKey(m_window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        rotate_sphere -= glm::vec3(sphere_rotate_speed, 0.f, 0.f);
    }
    if (glfwGetKey(m_window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        rotate_sphere += glm::vec3(0.f, sphere_rotate_speed, 0.f);
    }
    if (glfwGetKey(m_window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        rotate_sphere -= glm::vec3(0.f, sphere_rotate_speed, 0.f);
    }
    sphere_trsf->set_translation(sphere_trsf->get_translation() + translate_sphere);
    sphere_trsf->set_rotation(sphere_trsf->get_rotation() + rotate_sphere);
    if (!sphere_trsf->is_up_to_date()) sphere_trsf->compute();
}