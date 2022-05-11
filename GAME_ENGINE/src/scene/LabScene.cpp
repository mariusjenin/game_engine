
#include <src/physics/force/GravityForce.hpp>
#include <src/physics/rigid_body_behavior/MovementBehavior.hpp>
#include <src/physics/rigid_body_behavior/SwitchColorBehavior.hpp>
#include "LabScene.hpp"


using namespace scene;


void LabScene::setRoom(float scale, float mult, int id_tex) {
    TextureManager *texture_manager = m_shaders->get_texture_manager();

    auto *slab_mesh = new Mesh("../assets/mesh/slab.obj", true, OBB_TYPE);
    auto *roof_mesh = new Mesh("../assets/mesh/env/roof_lab.obj", true, OBB_TYPE);

    auto *floor_col = new MaterialColor({0.8, 0.8, 0.8}, 100);

    //floor
    auto *floor = new NodeGameSG(m_root, OBB_TYPE);
    floor->get_trsf()->set_translation({0, 0, 0});
    floor->get_trsf()->set_scale({scale, 2, 2 * scale});
    floor->set_meshes({slab_mesh});
    floor->set_material(new MaterialTexture(id_tex));
    floor->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto *floor_rbv = new RigidBodyVolume(floor);
    floor_rbv->add_behavior(new MovementBehavior(false, false, mult, 0, 0.2, 1));
    m_physics_system->add_collider(floor_rbv);

    //walls
    auto *wall = new NodeGameSG(m_root, OBB_TYPE);
    wall->get_trsf()->set_translation({10 * scale, 10, 0});
    wall->get_trsf()->set_scale({1, 2, scale * 1.5});
    wall->get_trsf()->set_rotation({0, 0, -90});
    wall->set_meshes({slab_mesh});
    wall->set_material(new MaterialColor({0.79, 0.3, 0.3}, 50));
    wall->set_material(new MaterialTexture(id_tex));
    wall->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto *wall_rbv = new RigidBodyVolume(wall);
    wall_rbv->add_behavior(new MovementBehavior(false, false, mult, 0));
    m_physics_system->add_collider(wall_rbv);

    auto *wall_1 = new NodeGameSG(m_root, OBB_TYPE);
    wall_1->get_trsf()->set_translation({-10 * scale, 10, 0});
    wall_1->get_trsf()->set_scale({1, 2, scale * 1.5});
    wall_1->get_trsf()->set_rotation({0, 0, -90});
    wall_1->set_meshes({slab_mesh});
    wall_1->set_material(new MaterialTexture(id_tex));
    wall_1->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto *wall_1_rbv = new RigidBodyVolume(wall_1);
    wall_1_rbv->add_behavior(new MovementBehavior(false, false, mult, 0));
    m_physics_system->add_collider(wall_1_rbv);

    auto *wall_2 = new NodeGameSG(m_root, OBB_TYPE);
    wall_2->get_trsf()->set_translation({0, 10, 10 * scale});
    wall_2->get_trsf()->set_scale({1, 2, scale * 1.5});
    wall_2->get_trsf()->set_rotation({0, 90, -90});
    wall_2->set_meshes({slab_mesh});
    wall_2->set_material(new MaterialTexture(id_tex));
    wall_2->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto *wall_2_rbv = new RigidBodyVolume(wall_2);
    wall_2_rbv->add_behavior(new MovementBehavior(false, false, mult, 0));
    m_physics_system->add_collider(wall_2_rbv);

    auto *wall_3 = new NodeGameSG(m_root, OBB_TYPE);
    wall_3->get_trsf()->set_translation({0, 10, -10 * scale});
    wall_3->get_trsf()->set_scale({1, 2, scale * 1.5});
    wall_3->get_trsf()->set_rotation({0, 90, -90});
    wall_3->set_meshes({slab_mesh});
    wall_3->set_material(new MaterialTexture(id_tex));
    wall_3->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto *wall_3_rbv = new RigidBodyVolume(wall_3);
    wall_3_rbv->add_behavior(new MovementBehavior(false, false, mult, 0));
    m_physics_system->add_collider(wall_3_rbv);

    //roof
    auto *roof = new NodeGameSG(m_root, OBB_TYPE);
    roof->get_trsf()->set_rotation({0, 90, 0});
    roof->get_trsf()->set_translation({0, 22.8, 0});
    roof->get_trsf()->set_scale({scale * 2, 3, scale * 2});
    roof->set_meshes({roof_mesh});
    roof->set_material(floor_col);
    roof->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto *roof_rbv = new RigidBodyVolume(roof);
    roof_rbv->add_behavior(new MovementBehavior(false, false, mult, 0, 0.2, 1));
    m_physics_system->add_collider(roof_rbv);

}

void LabScene::cubePyramid(glm::vec3 center, float size, float mult_physics) {
    auto *cube_mesh = new Mesh(create_rectangle_cuboid({size, size, size}), true, OBB_TYPE);
    int pyramid_height = 5;
    float offset = size/2.0f;

    glm::vec3 translation;
    glm::vec3 line_start = center - glm::vec3((float) (pyramid_height - 1) / 2., 0, 0);

    for (int i = 0; i < pyramid_height; i++) {
        translation = line_start + glm::vec3(i * offset, i * size, 0);

        for (int j = 0; j < (pyramid_height - i); j++) {

            auto *cube_node = new NodeGameSG(m_root, OBB_TYPE);
            cube_node->set_meshes({cube_mesh});
            cube_node->get_trsf()->set_translation(translation);
//            cube_node->set_material(new MaterialColor({0.8, 0.8, 0.8}, 100));
            glm::vec3 color = {(float) rand() / (float) RAND_MAX, (float) rand() / (float) RAND_MAX,
                               (float) rand() / (float) RAND_MAX,};
            cube_node->set_material(new MaterialColor(color, 50));
            auto *rbv_cube = new RigidBodyVolume(cube_node);
            rbv_cube->add_behavior(new MovementBehavior(true, true, mult_physics, 0.1, 1.0, 0.0));
//            rbv_cube->get_movement_behavior()->add_force(new GravityForce());

            m_physics_system->add_collider(rbv_cube);

            translation += glm::vec3(size * offset, 0, 0);
            m_items.push_back(rbv_cube);

        }
    }

}


LabScene::LabScene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path,
                   float mult_physics
) : Scene(
        window, vertex_shader_path, fragment_shader_path, mult_physics) {

    //PHYSICS SYSTEM
    m_physics_system = new PhysicsSystem(m_root, mult_physics, 0.8f, 0.00001f, 10, EULER_TYPE);

    //BACKGROUND
    glClearColor(0.15f, 0.15f, 0.15f, 0.0f);

    TextureManager *texture_manager = m_shaders->get_texture_manager();
    //Light Sources
    auto *light_source = new SpotLight({0.1, 0.1, 0.1}, {0.7, 0.7, 0.7}, {0.8, 0.8, 0.8},
                                       texture_manager->get_and_increment_id_texture(), 55, 75, 1500);
    auto *light_source2 = new SpotLight({0.1, 0.1, 0.1}, {0.7, 0.7, 0.7}, {0.8, 0.8, 0.8},
                                        texture_manager->get_and_increment_id_texture(), 55, 75, 1500);
    auto *light_source3 = new SpotLight({0.1, 0.1, 0.1}, {0.7, 0.7, 0.7}, {0.8, 0.8, 0.8},
                                        texture_manager->get_and_increment_id_texture(), 55, 75, 1500);
    auto *light_source4 = new SpotLight({0.1, 0.1, 0.1}, {0.7, 0.7, 0.7}, {0.8, 0.8, 0.8},
                                        texture_manager->get_and_increment_id_texture(), 55, 75, 1500);

    //TEXTURES
    // int id_ball_texture = texture_manager->load_texture("../assets/texture/rock.bmp");
    int id_warp_texture = texture_manager->load_texture("../assets/texture/portal_texture.bmp");
    int id_room_texture = texture_manager->load_texture("../assets/texture/Floor10.bmp");
    int id_left_wall_texture = texture_manager->load_texture("../assets/texture/blue_texture.bmp");
    int id_right_wall_texture = texture_manager->load_texture("../assets/texture/red_texture.bmp");

    auto *cube_mesh = new Mesh(create_rectangle_cuboid({2, 2, 2}), true, OBB_TYPE);
    auto *ball_mesh = new Mesh(create_sphere(1.8, 60, 60), true, SPHEREBB_TYPE);
    auto *cube_mesh2 = new Mesh(create_rectangle_cuboid({2, 2, 3}), true, OBB_TYPE);
    auto *table_mesh = new Mesh("../assets/mesh/props/table_lab.obj", true, OBB_TYPE);

    //Light
//    auto *light_source = new DirectionLight({0.2, 0.2, 0.2}, {1., 1., 1.}, {0.8, 0.8, 0.8});
    auto *light_node = new NodeGameSG(m_root);
    light_node->get_trsf()->set_translation({-40, 20, 40});
    light_node->get_trsf()->set_rotation({-90, 0, 0});
    light_node->get_trsf()->set_order_rotation(ORDER_ZXY);
    light_node->set_light(light_source);
    m_lights.push_back(light_node);
    auto *light_node2 = new NodeGameSG(m_root);
    light_node2->get_trsf()->set_translation({-40, 20, -40});
    light_node2->get_trsf()->set_rotation({-90, 0, 0});
    light_node2->get_trsf()->set_order_rotation(ORDER_ZXY);
    light_node2->set_light(light_source2);
    m_lights.push_back(light_node2);
    auto *light_node3 = new NodeGameSG(m_root);
    light_node3->get_trsf()->set_translation({40, 20, 40});
    light_node3->get_trsf()->set_rotation({-90, 0, 0});
    light_node3->get_trsf()->set_order_rotation(ORDER_ZXY);
    light_node3->set_light(light_source3);
    m_lights.push_back(light_node3);
    auto *light_node4 = new NodeGameSG(m_root);
    light_node4->get_trsf()->set_translation({40, 20, -40});
    light_node4->get_trsf()->set_rotation({-90, 0, 0});
    light_node4->get_trsf()->set_order_rotation(ORDER_ZXY);
    light_node4->set_light(light_source4);
    m_lights.push_back(light_node4);

    //Basic room setup
    float scale = 10;
    setRoom(scale, mult_physics, id_room_texture);


    //PROPS IN THE ROOM
    //cube
    auto *big_ball = new NodeGameSG(m_root, OBB_TYPE);
    big_ball->get_trsf()->set_translation({-10, 20, 15});
    big_ball->get_trsf()->set_rotation({46, 12, 33});
    big_ball->set_meshes({ball_mesh});
    big_ball->set_material(new MaterialColor({0.75, 0.3, 0.95}, 50));
    // big_ball->set_debug_rendering(true, {0.85, 0.5, 1});


    auto* bad_balls_mat = new MaterialColor({0.8, 0.2, 0.2}, 10);
    auto* good_ball_mat = new MaterialColor({0.2, 0.8, 0.2}, 10);
    //ball
    auto *ball_1 = new NodeGameSG(m_root, SPHEREBB_TYPE);
    ball_1->get_trsf()->set_translation({37., 10, 3});
    ball_1->set_meshes({ball_mesh});
    ball_1->set_material(bad_balls_mat);
    auto *ball_2 = new NodeGameSG(m_root, SPHEREBB_TYPE);
    ball_2->get_trsf()->set_translation({37., 10, -3});
    ball_2->set_meshes({ball_mesh});
    ball_2->set_material(bad_balls_mat);
    auto *ball_3 = new NodeGameSG(m_root, SPHEREBB_TYPE);
    ball_3->get_trsf()->set_translation({43, 10, -3});
    ball_3->set_meshes({ball_mesh});
    ball_3->set_material(bad_balls_mat);
    auto *ball_4 = new NodeGameSG(m_root, SPHEREBB_TYPE);
    ball_4->get_trsf()->set_translation({43, 10, 3});
    ball_4->set_meshes({ball_mesh});
    ball_4->set_material(bad_balls_mat);
    auto *ball_5 = new NodeGameSG(m_root, SPHEREBB_TYPE);
    ball_5->get_trsf()->set_translation({40, 10, 0});
    ball_5->set_meshes({ball_mesh});
    ball_5->set_material(good_ball_mat);

    auto *table = new NodeGameSG(m_root, OBB_TYPE);
    table->get_trsf()->set_translation({40., 5.5, 0});
    table->get_trsf()->set_scale({2, 2, 2});
    table->set_meshes({table_mesh});
    table->set_material(new MaterialColor({0.75, 0.3, 0.95}, 50));
    // table->set_debug_rendering(true);

    glm::vec3 pyra_pos(-scale * 4, 2, 0);
    cubePyramid(pyra_pos, 2, mult_physics);

    auto *gravity_force = new GravityForce();
    auto *rbv_big_ball = new RigidBodyVolume(big_ball);
    auto *rbv_ball_1 = new RigidBodyVolume(ball_1);
    auto *rbv_ball_2 = new RigidBodyVolume(ball_2);
    auto *rbv_ball_3 = new RigidBodyVolume(ball_3);
    auto *rbv_ball_4 = new RigidBodyVolume(ball_4);
    auto *rbv_ball_5 = new RigidBodyVolume(ball_5);
    auto *rbv_table = new RigidBodyVolume(table);

    rbv_big_ball->add_behavior(new MovementBehavior(true, true, mult_physics, 4, 0.8, 0.5));
    rbv_ball_1->add_behavior(new MovementBehavior(true, true, mult_physics, 1, 0.8, 0.5));
    rbv_ball_2->add_behavior(new MovementBehavior(true, true, mult_physics, 1, 0.8, 0.5));
    rbv_ball_3->add_behavior(new MovementBehavior(true, true, mult_physics, 1, 0.8, 0.5));
    rbv_ball_4->add_behavior(new MovementBehavior(true, true, mult_physics, 1, 0.8, 0.5));
    rbv_ball_5->add_behavior(new MovementBehavior(true, true, mult_physics, 1, 0.8, 0.5));
    rbv_table->add_behavior(new MovementBehavior(false, false, mult_physics, 0));

    rbv_big_ball->get_movement_behavior()->add_force(gravity_force);
    rbv_ball_1->get_movement_behavior()->add_force(gravity_force);
    rbv_ball_2->get_movement_behavior()->add_force(gravity_force);
    rbv_ball_3->get_movement_behavior()->add_force(gravity_force);
    rbv_ball_4->get_movement_behavior()->add_force(gravity_force);
    rbv_ball_5->get_movement_behavior()->add_force(gravity_force);

    m_physics_system->add_collider(rbv_table);
    m_physics_system->add_collider(rbv_big_ball);
    m_physics_system->add_collider(rbv_ball_1);
    m_physics_system->add_collider(rbv_ball_2);
    m_physics_system->add_collider(rbv_ball_3);
    m_physics_system->add_collider(rbv_ball_4);
    m_physics_system->add_collider(rbv_ball_5);
    m_physics_system->add_collider(rbv_table);
//    auto* rbv_cube2 = new RigidBodyVolume(m_cube2,1000,0.01,1);
//    rbv_cube2->add_force(gravity_force);
//    m_physics_system.add_collider(rbv_cube2);

    //CHARACTER
    m_character = new Character(m_root, m_physics_system);
    m_cameras.push_back(m_character->get_camera());
    m_physics_system->add_collider(m_character->get_body());

//    //CAMERA
    auto *camera_node = new NodeGameSG(m_root);
    camera_node->get_trsf()->set_translation({0, 9, 17});
    camera_node->get_trsf()->set_rotation({-20, 0, 0});
    m_cameras.push_back(camera_node);


    //SCENE ITEMS (grabbable items (=props))
    m_items.push_back(rbv_big_ball);
    m_items.push_back(rbv_ball_1);
    m_items.push_back(rbv_ball_2);
    m_items.push_back(rbv_ball_3);
    m_items.push_back(rbv_ball_4);
    m_items.push_back(rbv_ball_5);


    //DOORS
    auto *door_1 = new DoorElement(m_root, OBB_TYPE, id_left_wall_texture, id_right_wall_texture);
    door_1->add_behavior(mult_physics);
    door_1->get_trsf()->set_translation({0, 11, 0});
    m_physics_system->add_collider(door_1->get_right_rigid_body());
    m_physics_system->add_collider(door_1->get_left_rigid_body());

    //OUT OF SCENE
    auto *door_2 = new DoorElement(m_root, OBB_TYPE, id_left_wall_texture, id_right_wall_texture);
    door_2->add_behavior(mult_physics);
    door_2->get_trsf()->set_translation({-scale * 9.8, 11, 0});
    m_physics_system->add_collider(door_2->get_right_rigid_body());
    m_physics_system->add_collider(door_2->get_left_rigid_body());


    //DOORWALLS
    glm::vec3 door_pos = door_1->get_trsf()->get_translation();

    auto *wall_left = new NodeGameSG(m_root, OBB_TYPE);
    wall_left->get_trsf()->set_scale({scale * 5, scale * 2, 1});
    wall_left->get_trsf()->set_rotation({0, 90, 0});
    wall_left->get_trsf()->set_translation(door_pos + glm::vec3(-2, 0, scale * 5.9));
    wall_left->set_meshes({cube_mesh2});
    wall_left->set_material(new MaterialColor({0.79, 0.3, 0.3}, 50));
    auto *wall_rbv = new RigidBodyVolume(wall_left);
    wall_rbv->add_behavior(new MovementBehavior(false, false, mult_physics, 0));
    m_physics_system->add_collider(wall_rbv);

    auto *wall_right = new NodeGameSG(m_root, OBB_TYPE);
    wall_right->get_trsf()->set_scale({scale * 5, scale * 2, 1});
    wall_right->get_trsf()->set_rotation({0, 90, 0});
    wall_right->get_trsf()->set_translation(door_pos + glm::vec3(-2, 0, -scale * 5.5));
    wall_right->set_meshes({cube_mesh2});
    wall_right->set_material(new MaterialColor({0.79, 0.3, 0.3}, 50));
    auto *wall_rbv_2 = new RigidBodyVolume(wall_right);
    wall_rbv_2->add_behavior(new MovementBehavior(false, false, mult_physics, 0));
    m_physics_system->add_collider(wall_rbv_2);

    //BTN
    auto *button_1 = new ButtonElement(m_root, OBB_TYPE);
    button_1->get_trsf()->set_rotation({0, 0, -90});
    button_1->get_trsf()->set_translation(door_pos + glm::vec3(-scale * 3, 0, scale * 1.5));
    button_1->get_trsf()->set_uniform_scale(2.f);
    m_physics_system->add_collider(button_1->get_rigid_body());
    button_1->link_door(door_1);
    button_1->add_behavior(mult_physics,{rbv_ball_5});

    door_pos = door_2->get_trsf()->get_translation();
    auto *button_2 = new ButtonElement(m_root, OBB_TYPE);
    button_2->get_trsf()->set_rotation({0, 0, -90});
    button_2->get_trsf()->set_translation(door_pos + glm::vec3(-scale * 3, 0, scale * 1.5));
    button_2->get_trsf()->set_uniform_scale(2.f);
    m_physics_system->add_collider(button_2->get_rigid_body());
    button_2->link_door(door_2);
    button_2->add_behavior(mult_physics,{rbv_big_ball});

    //WARP (change scene trigger)
    auto *warp = new NodeGameSG(m_root, OBB_TYPE);
    warp->get_trsf()->set_translation(door_pos + glm::vec3(-2, 0, 2.6));
    warp->set_meshes({cube_mesh});
    warp->get_trsf()->set_scale({7, 9, 1});
    warp->get_trsf()->set_rotation({0, 90, 0});
    warp->set_material(new MaterialTexture(id_warp_texture));
    auto *warp_rbv = new RigidBodyVolume(warp);
    warp_rbv->add_behavior(new MovementBehavior(false, false, mult_physics, 0));
    m_physics_system->add_collider(warp_rbv);

    //PROJECTION
    m_fovy = 65.0f;
    m_z_near = 0.1f;
    m_z_far = 10000.0f;


    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //MOUSE PROCESSING
    // glfwSetCursorPosCallback(window, process_mouse);
    // Character* character = scene.get_character();
    glfwSetCursorPosCallback(window, MouseView::process_mouse);

    m_camera_index = 0;
    m_timing_camera_switch = 0;
}

std::vector<RigidBodyVolume *> LabScene::get_items() {
    return m_items;
}


RigidBodyVolume *LabScene::in_sight() {
    //RAYCAST character's sight grabbable items in scene
    Transform *cam_trsf = m_character->get_camera()->get_trsf();
    glm::vec3 sight = glm::vec3(0, 0, -1);
    sight = cam_trsf->apply_to_vector(sight);

    glm::vec3 char_pos = m_character->get_camera()->get_position_in_world();

    RigidBodyVolume *rbv_result = nullptr;

    Ray ray(
            char_pos,
            glm::normalize(sight)
    );

    float t = FLT_MAX;
    for (RigidBodyVolume *rbv: m_items) {
        glm::vec3 translation = rbv->get_node()->get_trsf()->get_translation();
        float inter = rbv->get_node()->get_bb()->is_intersected(ray);

        //Get closest items
        if (inter > 0.f) {
            if (inter < t) {
                t = inter;
                rbv_result = rbv;
            }
        }
    }

    return rbv_result;
}


void LabScene::update(float delta_time) {

    Scene::update(delta_time);

    //CAMERA
    NodeGameSG *camera_node = m_cameras[m_camera_index];

    //Camera node rotates along y axis.
    float yaw = m_character->get_mouse_view()->get_yaw();
    float pitch = m_character->get_mouse_view()->get_pitch();

    camera_node->get_trsf()->set_rotation({pitch, -yaw, 0});
    camera_node->compute_trsf_scene_graph();

}

void LabScene::process_input(float delta_time) {

    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    float camera_speed = 15.f * delta_time;
    float camera_speed_rot = 150 * delta_time;
    float cube_translate_speed = 15 * delta_time;
    float character_speed = 35.f * delta_time;
    double timestamp = glfwGetTime();

    Transform *character_trsf = m_character->get_character_node()->get_trsf();
    Transform *character_cam_trsf = m_character->get_camera()->get_trsf();
    // Transform *cube_trsf = m_cube->get_trsf();

    //Compute translation relative to camera direction
    glm::vec3 forward(0, 0, -1);
    glm::vec3 forward_vec = character_cam_trsf->apply_to_vector(forward);

    forward_vec[1] = 0.;    //disable flight
    forward_vec = glm::normalize(forward_vec);

    glm::vec3 up(0, 1, 0);
    glm::vec3 right_vec = glm::cross(forward_vec, up);

    m_timing_camera_switch -= delta_time;
    if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS && m_timing_camera_switch <= 0) {
        m_timing_camera_switch = 1;
        m_camera_index++;
        if (m_camera_index == m_cameras.size()) m_camera_index = 0;
    }

    glm::vec3 dir;
    //Camera Translation
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            dir += character_speed * right_vec;
        }
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            dir += -character_speed * right_vec;

        }
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            dir += -character_speed * forward_vec;
        }
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            dir += character_speed * forward_vec;
        }
        if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            m_character->jump();
        }
        character_trsf->set_translation(character_trsf->get_translation() + dir);
    }
    if (!character_trsf->is_up_to_date()) character_trsf->compute();

    //Be sure to get only 1 input
    if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
        if (m_character->has_item())
            m_character->accumulate_power();

        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_RELEASE) {
            bool available = m_character->can_interact(timestamp);

            if (m_character->has_item() && available) {

                m_character->throw_item(timestamp);

            } else {

                //GRAB ITEM
                RigidBodyVolume *rbv = in_sight();
                if (rbv != nullptr && available) {
                    m_character->grab_item(rbv, timestamp);
                    glm::vec3 forces = rbv->get_movement_behavior()->get_forces();
                }
            }
        }

    }

    // glm::vec3 translate_cube;
    // bool impulse_cube = false;
    // //Scene rotation
    // if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    //     translate_cube += glm::vec3(cube_translate_speed, 0.f, 0.f);
    //     impulse_cube =  true;
    // }
    // if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    //     translate_cube -= glm::vec3(cube_translate_speed, 0.f, 0.f);
    //     impulse_cube =  true;
    // }
    // if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    //     translate_cube += glm::vec3(0.f, 0.f, cube_translate_speed);
    //     impulse_cube =  true;
    // }
    // if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
    //     translate_cube -= glm::vec3(0.f, 0.f, cube_translate_speed);
    //     impulse_cube =  true;
    // }
    // translate_cube *= 10.f;
    // if(impulse_cube && m_cube->get_rigid_body()->has_movement_behavior())m_cube->get_rigid_body()->get_movement_behavior()->set_velocity(translate_cube);

    if (m_camera_index == 1) {
        glm::vec3 translate_camera_free = {0, 0, 0};
        //Scene rotation
        if (glfwGetKey(m_window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
            translate_camera_free += glm::vec3(camera_speed, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_K) == GLFW_PRESS) {
            translate_camera_free -= glm::vec3(camera_speed, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_L) == GLFW_PRESS) {
            translate_camera_free += glm::vec3(0.f, 0.f, camera_speed);
        }
        if (glfwGetKey(m_window, GLFW_KEY_O) == GLFW_PRESS) {
            translate_camera_free -= glm::vec3(0.f, 0.f, camera_speed);
        }
        auto *camera_trsf = m_cameras[m_camera_index]->get_trsf();
        camera_trsf->set_translation(
                camera_trsf->get_translation() + camera_trsf->apply_to_vector(translate_camera_free));
    }


    // cube_trsf->set_translation(cube_trsf->get_translation() + translate_cube);
    // if (!cube_trsf->is_up_to_date()) cube_trsf->compute();

    //DEBUG
    // glm::vec3 chara_pos = character_trsf->get_translation();
    // std::cout<<"CHARACTER POS: "<<chara_pos[0]<<","<<chara_pos[1]<<","<<chara_pos[2]<<std::endl;
}

