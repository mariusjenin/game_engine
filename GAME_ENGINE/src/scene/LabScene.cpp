
#include <src/physics/force/GravityForce.hpp>
#include <src/physics/rigid_body_behavior/MovementBehavior.hpp>
#include <src/physics/rigid_body_behavior/SwitchColorBehavior.hpp>
#include "LabScene.hpp"


using namespace scene;

void LabScene::setRoom(float scale, float mult){
    TextureManager *texture_manager = m_shaders->get_texture_manager();

    auto *slab_mesh = new Mesh("../assets/mesh/slab.obj", true, OBB_TYPE);
    auto *roof_mesh = new Mesh("../assets/mesh/env/roof_lab.obj", true, OBB_TYPE);

    int id_floor_texture = texture_manager->load_texture("../assets/texture/Floor10.bmp");

    auto* floor_col = new MaterialColor(m_shaders, {0.8, 0.8, 0.8}, 100);

     //floor
    auto* floor = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    floor->get_trsf()->set_translation({0,0,0});
    floor->get_trsf()->set_scale({scale, 2, 2*scale});
    floor->set_meshes({slab_mesh});
    floor->set_material(new MaterialTexture(m_shaders, id_floor_texture));
    floor->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto * floor_rbv = new RigidBodyVolume(floor);
    floor_rbv->add_behavior(new MovementBehavior(false,false,mult,0,0.2,1));
    m_physics_system->add_collider(floor_rbv);

    //walls
    auto* wall = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    wall->get_trsf()->set_translation({10*scale,10,0});
    wall->get_trsf()->set_scale({1, 2, scale*1.5});
    wall->get_trsf()->set_rotation({0, 0, -90});
    wall->set_meshes({slab_mesh});
    wall->set_material(new MaterialColor(m_shaders, {0.79, 0.3, 0.3}, 50));
    wall->set_material(new MaterialTexture(m_shaders, id_floor_texture));
    wall->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto * wall_rbv = new RigidBodyVolume(wall);
    wall_rbv->add_behavior(new MovementBehavior(false,false,mult,0));
    m_physics_system->add_collider(wall_rbv);

    auto* wall_1 = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    wall_1->get_trsf()->set_translation({-10*scale,10,0});
    wall_1->get_trsf()->set_scale({1, 2, scale*1.5});
    wall_1->get_trsf()->set_rotation({0, 0, -90});
    wall_1->set_meshes({slab_mesh});
    wall_1->set_material(new MaterialTexture(m_shaders, id_floor_texture));
    wall_1->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto * wall_1_rbv = new RigidBodyVolume(wall_1);
    wall_1_rbv->add_behavior(new MovementBehavior(false,false,mult,0));
    m_physics_system->add_collider(wall_1_rbv);
    
    auto* wall_2 = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    wall_2->get_trsf()->set_translation({0,10,10*scale});
    wall_2->get_trsf()->set_scale({1, 2, scale*1.5});
    wall_2->get_trsf()->set_rotation({0, 90, -90});
    wall_2->set_meshes({slab_mesh});
    wall_2->set_material(new MaterialTexture(m_shaders, id_floor_texture));
    wall_2->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto * wall_2_rbv = new RigidBodyVolume(wall_2);
    wall_2_rbv->add_behavior(new MovementBehavior(false,false,mult,0));
    m_physics_system->add_collider(wall_2_rbv);
    
    auto* wall_3 = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    wall_3->get_trsf()->set_translation({0,10,-10*scale});
    wall_3->get_trsf()->set_scale({1, 2, scale*1.5});
    wall_3->get_trsf()->set_rotation({0, 90, -90});
    wall_3->set_meshes({slab_mesh});    
    wall_3->set_material(new MaterialTexture(m_shaders, id_floor_texture));
    wall_3->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto * wall_3_rbv = new RigidBodyVolume(wall_3);
    wall_3_rbv->add_behavior(new MovementBehavior(false,false,mult,0));
    m_physics_system->add_collider(wall_3_rbv);

    //roof
    auto* roof = new NodeGameSG(m_shaders, m_root, OBB_TYPE);
    roof->get_trsf()->set_rotation({0, 90, 0});
    roof->get_trsf()->set_translation({0,25,0});
    roof->get_trsf()->set_scale({scale*2, 3, scale*2});
    roof->set_meshes({roof_mesh});
    roof->set_material(floor_col);
    roof->set_debug_rendering(true, {0.25, 0.65, 0.8});
    auto * roof_rbv = new RigidBodyVolume(roof);
    roof_rbv->add_behavior(new MovementBehavior(false,false,mult,0,0.2,1));
    m_physics_system->add_collider(roof_rbv);

}

void LabScene::cubePyramid(glm::vec3 center, float size, float mult_physics){
    auto *cube_mesh = new Mesh(create_rectangle_cuboid({size,size,size}), true, OBB_TYPE);
    int pyramid_height = 5;
    float offset = 0.1;
    
    glm::vec3 translation; 
    glm::vec3 line_start = center - glm::vec3((float) (pyramid_height-1)/2., 0, 0);

    for(int i = 0; i < pyramid_height; i ++){
        translation = line_start + glm::vec3(i*offset, i*size*offset, 0);

        for(int j = 0; j < (pyramid_height-i); j ++){

            auto* cube_node = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
            cube_node->set_meshes({cube_mesh});
            cube_node->get_trsf()->set_translation(translation);
            cube_node->set_material(new MaterialColor(m_shaders, {0.8, 0.8, 0.8}, 100));
            RigidBodyVolume* rbv_cube = new RigidBodyVolume(cube_node);
            rbv_cube->add_behavior(new MovementBehavior(true,true,mult_physics, 0.5, 0.8, 0.5));
            // rbv_cube->get_movement_behavior()->add_force(new GravityForce());
            
            m_physics_system->add_collider(rbv_cube);

            translation += glm::vec3(size*offset, 0, 0);
            m_items.push_back(rbv_cube);

        }
    }

}


LabScene::LabScene(const std::string &vertex_shader_path, const std::string &fragment_shader_path, float mult_physics) : Scene(
        vertex_shader_path, fragment_shader_path, mult_physics)  {

    //PHYSICS SYSTEM
    m_physics_system = new PhysicsSystem(m_root, mult_physics,0.2f, 0.00001f, 5,EULER_TYPE);
    TextureManager *texture_manager = m_shaders->get_texture_manager();
    
    //BACKGROUND
    glClearColor(0.15f, 0.15f, 0.15f, 0.0f);

    //TEXTURES
    int id_ball_texture = texture_manager->load_texture("../assets/texture/rock.bmp");
    // int id_floor_texture = texture_manager->load_texture("../assets/texture/Floor10.bmp");

    //MESHES
    auto *btn1_mesh = new Mesh("../assets/mesh/props/button_1.obj", true, OBB_TYPE);
    auto *btn2_mesh = new Mesh("../assets/mesh/props/button_2.obj", true, OBB_TYPE);
    auto *cube_mesh = new Mesh(create_rectangle_cuboid({5,5,5}), true,OBB_TYPE);
    auto *cube_mesh_floor = new Mesh(create_rectangle_cuboid({20,5,20}), true,OBB_TYPE);
    auto *ball_mesh1 = new Mesh(create_sphere(1, 60, 60),true,SPHEREBB_TYPE);
    auto *cube_mesh2 = new Mesh(create_rectangle_cuboid({1,1,1}), true,OBB_TYPE);
    auto *table_mesh= new Mesh("../assets/mesh/props/table_lab.obj", true, OBB_TYPE);

    //Light
    auto *light_source = new DirectionLight({0.2, 0.2, 0.2}, {1., 1., 1.}, {0.8, 0.8, 0.8}, {-0.2, -1., -0.5});
    auto *light_node = new NodeGameSG(m_shaders, m_root);
    light_node->set_light(light_source);
    m_lights.push_back(light_node);

    //Basic room setup
    float scale = 10;    
    setRoom(scale, mult_physics);


    //PROPS IN THE ROOM
    //cube
    auto* cube = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    cube->get_trsf()->set_translation({1,20,0});
    cube->get_trsf()->set_rotation({46,12,33});
    cube->get_trsf()->set_uniform_scale(3);
    cube->set_meshes({cube_mesh2});
    cube->set_material(new MaterialColor(m_shaders, {0.75, 0.3, 0.95}, 50));
    // m_cube->set_debug_rendering(true, {0.85, 0.5, 1});

    //ball
    auto* m_ball = new NodeGameSG(m_shaders, m_root,SPHEREBB_TYPE);
    m_ball->get_trsf()->set_translation({5.,10,0});
    m_ball->set_meshes({ball_mesh1});
    // m_ball->set_material(new MaterialColor(m_shaders, {0.75, 0.3, 0.95}, 50));
    m_ball->set_material(new MaterialTexture(m_shaders, id_ball_texture));
    // m_ball->set_debug_rendering(true);
    
    //TEST PROPS
    // auto* btn1 = new NodeGameSG(m_shaders, m_root, SPHEREBB_TYPE);
    // btn1->get_trsf()->set_translation({15.,10,0});
    // btn1->get_trsf()->set_uniform_scale(2);
    // btn1->set_meshes({btn1_mesh});
    // btn1->set_material(new MaterialTexture(m_shaders, id_ball_texture));
    
    // auto* btn2 = new NodeGameSG(m_shaders, m_root, SPHEREBB_TYPE);
    // btn2->get_trsf()->set_translation({15.,10,0});
    // btn2->get_trsf()->set_uniform_scale(2);
    // btn2->set_meshes({btn2_mesh});
    // btn2->set_material(new MaterialTexture(m_shaders, id_ball_texture));
    // m_ball->set_debug_rendering(true);

   
    auto* table = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    table->get_trsf()->set_translation({40.,5.5,0});
    table->get_trsf()->set_scale({2, 2, 2});
    table->set_meshes({table_mesh});
    table->set_material(new MaterialColor(m_shaders, {0.75, 0.3, 0.95}, 50));
    table->set_debug_rendering(true);

    cubePyramid(glm::vec3(-40, 2, 0), 2, mult_physics);

    auto* gravity_force = new GravityForce();
    auto* rbv_cube = new RigidBodyVolume(cube);
    auto* rbv_sphere = new RigidBodyVolume(m_ball);
    auto* rbv_table = new RigidBodyVolume(table);

    rbv_cube->add_behavior(new MovementBehavior(true,true,mult_physics,1, 0.8, 0.5));
    rbv_sphere->add_behavior(new MovementBehavior(true,true,mult_physics,1, 0.8, 0.5));
    rbv_table->add_behavior(new MovementBehavior(false,false,mult_physics,0));

    rbv_cube->get_movement_behavior()->add_force(gravity_force);
    rbv_sphere->get_movement_behavior()->add_force(gravity_force);
    
    m_physics_system->add_collider(rbv_table);
    m_physics_system->add_collider(rbv_cube);
    m_physics_system->add_collider(rbv_sphere);
//    auto* rbv_cube2 = new RigidBodyVolume(m_cube2,1000,0.01,1);
//    rbv_cube2->add_force(gravity_force);
//    m_physics_system.add_collider(rbv_cube2);

    //CHARACTER
    m_character = new Character(m_shaders, m_root, m_physics_system);
    m_cameras.push_back(m_character->get_camera());
    m_physics_system->add_collider(m_character->get_body());

   //CAMERA
   auto *camera_node = new NodeGameSG(m_shaders, m_root);
   camera_node->get_trsf()->set_translation({0, 9, 17});
   camera_node->get_trsf()->set_rotation({-20, 0, 0});
   m_cameras.push_back(camera_node);


    //SCENE ITEMS (grabbable items (=props))
    m_items.push_back(rbv_cube);
    m_items.push_back(rbv_sphere);

    //DOOR
    DoorElement* door = new DoorElement(m_shaders, m_root, OBB_TYPE);
    door->add_behavior(mult_physics);
    door->get_trsf()->set_translation({0,11,0});
    // door->get_trsf()->set_rotation({0, 90, 0});
    m_physics_system->add_collider(door->get_right());
    m_physics_system->add_collider(door->get_left());

    door->get_right()->get_node()->set_debug_rendering(true);
    // m_doors.push_back(door);
    
    //BTN
    ButtonElement* button = new ButtonElement(m_shaders, m_root, OBB_TYPE);
    button->get_trsf()->set_translation({-15, 2, 20});
    m_physics_system->add_collider(button->get_top());
    button->link_door(door);
    button->add_behavior(mult_physics);

    m_buttons.push_back(button);
    
    //PROJECTION
    mat4 projection_mat = perspective(radians(65.0f), 4.f / 3.0f, 0.1f, 10000.0f);
    glUniformMatrix4fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::PROJ_MAT_LOC_NAME), 1,
                       GL_FALSE, &projection_mat[0][0]);

   m_camera_index = 0;
   m_timing_camera_switch = 0;
}

std::vector<RigidBodyVolume*> LabScene::get_items(){
    return m_items;
}


RigidBodyVolume* LabScene::in_sight(){
    //RAYCAST character's sight grabbable items in scene
    Transform* cam_trsf = m_character->get_camera()->get_trsf();
    glm::vec3 sight = glm::vec3(0,0,-1);
    sight = cam_trsf->apply_to_vector(sight);

    glm::vec3 char_pos =  m_character->get_camera()->get_position_in_world();

    //DEBUG 
    // std::cout<<"sight: "<<sight[0]<<", "<<sight[1]<<", "<<sight[2]<<std::endl;
    // std::cout<<"char_pos: "<<char_pos[0]<<", "<<char_pos[1]<<", "<<char_pos[2]<<std::endl;

    RigidBodyVolume* rbv_result = nullptr;
    
    Ray ray(
        char_pos, 
        glm::normalize(sight)
    );

    float t = FLT_MAX;
    for(RigidBodyVolume* rbv : m_items){
        glm::vec3 translation = rbv->get_node()->get_trsf()->get_translation();
        float inter = rbv->get_node()->get_bb()->is_intersected(ray);

        //Get closest items
        if(inter > 0.f){
            if(inter < t){
                t = inter;
                rbv_result = rbv;
            }
        }
    }
        
    return rbv_result;
}


void LabScene::update(GLFWwindow *window, float delta_time){

    Scene::update(window, delta_time);

    //CAMERA
    NodeGameSG *camera_node = m_cameras[m_camera_index];
    
    //Camera node rotates along y axis.
    float yaw = m_character->m_mouse_view->get_yaw();
    float pitch = m_character->m_mouse_view->get_pitch();

    camera_node->get_trsf()->set_rotation({pitch, -yaw, 0});
    camera_node->compute_trsf_scene_graph();
        
}

void LabScene::process_input(GLFWwindow *window, float delta_time) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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
   if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && m_timing_camera_switch <= 0) {
       m_timing_camera_switch = 1;
       m_camera_index++;
       if(m_camera_index == m_cameras.size()) m_camera_index = 0;
   }

    glm::vec3 dir;
    //Camera Translation
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            dir += character_speed * right_vec;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            dir += -character_speed * right_vec;

        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            dir += -character_speed * forward_vec;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            dir += character_speed * forward_vec;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            m_character->jump();
        }
        character_trsf->set_translation(character_trsf->get_translation() + dir);
    }
    if (!character_trsf->is_up_to_date()) character_trsf->compute();

    //Be sure to get only 1 input
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if(m_character->has_item)
            m_character->accumulate_power();

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
            bool available = m_character->can_interact(timestamp);
            
            if(m_character->has_item && available){

                m_character->throw_item(timestamp);
                
            }else{

                //GRAB ITEM
                RigidBodyVolume* rbv = in_sight();
                if(rbv != nullptr && available){
                    m_character->grab_item(rbv, timestamp);
                    glm::vec3 forces = rbv->get_movement_behavior()->get_forces();
                }
            }
        }

    }

    // glm::vec3 translate_cube;
    // bool impulse_cube = false;
    // //Scene rotation
    // if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    //     translate_cube += glm::vec3(cube_translate_speed, 0.f, 0.f);
    //     impulse_cube =  true;
    // }
    // if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    //     translate_cube -= glm::vec3(cube_translate_speed, 0.f, 0.f);
    //     impulse_cube =  true;
    // }
    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    //     translate_cube += glm::vec3(0.f, 0.f, cube_translate_speed);
    //     impulse_cube =  true;
    // }
    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    //     translate_cube -= glm::vec3(0.f, 0.f, cube_translate_speed);
    //     impulse_cube =  true;
    // }
    // translate_cube *= 10.f;
    // if(impulse_cube && m_cube->get_rigid_body()->has_movement_behavior())m_cube->get_rigid_body()->get_movement_behavior()->set_velocity(translate_cube);

   if(m_camera_index == 1){
       glm::vec3 translate_camera_free = {0,0,0};
       //Scene rotation
       if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
           translate_camera_free += glm::vec3(camera_speed, 0.f, 0.f);
       }
       if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
           translate_camera_free -= glm::vec3(camera_speed, 0.f, 0.f);
       }
       if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
           translate_camera_free += glm::vec3(0.f, 0.f, camera_speed);
       }
       if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
           translate_camera_free -= glm::vec3(0.f, 0.f, camera_speed);
       }
       auto* camera_trsf = m_cameras[m_camera_index]->get_trsf();
       camera_trsf->set_translation(camera_trsf->get_translation() + camera_trsf->apply_to_vector(translate_camera_free));
   }


    // cube_trsf->set_translation(cube_trsf->get_translation() + translate_cube);
    // if (!cube_trsf->is_up_to_date()) cube_trsf->compute();

    //DEBUG
    // glm::vec3 chara_pos = character_trsf->get_translation();
    // std::cout<<"CHARACTER POS: "<<chara_pos[0]<<","<<chara_pos[1]<<","<<chara_pos[2]<<std::endl;
}

Character* LabScene::get_character(){
    return m_character;
}


