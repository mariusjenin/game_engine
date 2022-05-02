
#include <src/physics/force/GravityForce.hpp>
#include "LabScene.hpp"


using namespace scene;

LabScene::LabScene(const std::string &vertex_shader_path, const std::string &fragment_shader_path) : Scene(
        vertex_shader_path, fragment_shader_path) {

    //PHYSICS SYSTEM
    m_physics_system = new PhysicsSystem(0.0005f, 0.005f, 5);        
    //BACKGROUND
    glClearColor(0.15f, 0.15f, 0.15f, 0.0f);

    //TEXTURES
    int id_texture = 0;
//    int id_land_texture = id_texture++;
//    load_bmp_custom("../assets/texture/rock.bmp", id_land_texture);
    //MESHES
    auto *slab_mesh = new Mesh("../assets/mesh/slab.obj", true, OBB_TYPE);
    auto *cube_mesh = new Mesh(create_rectangle_cuboid({5,5,5}), true,OBB_TYPE);
    auto *ball_mesh1 = new Mesh(create_sphere(1, 60, 60),true,SphereBB_TYPE);
    auto *cube_mesh2 = new Mesh(create_rectangle_cuboid({1,1,1}), true,OBB_TYPE);
//    auto *plane_mesh1 = new LODMesh(create_plane(100, 100, {-10, 0, -10}, {10, 0, 10}, Y_NORMAL_DIRECTION), 2, 30, 60, 5, 10,AABB_TYPE);
//    auto *plane_mesh2 = new LODMesh(create_plane(100, 100, {-10, 0, -10}, {10, 0, 10}, Y_INV_NORMAL_DIRECTION), 2, 30, 60, 5, 10,AABB_TYPE);

    //Light
    auto *light_source = new DirectionLight({0.1, 0.1, 0.1}, {1., 1., 1.}, {0.8, 0.8, 0.8}, {-0.2, -1., -0.5});
    auto *light_node = new NodeGameSG(m_shaders, m_root);
    light_node->set_light(light_source);
    m_lights.push_back(light_node);

    auto* lab_mat_color = new MaterialColor(m_shaders, {0.15, 0.55, 0.7}, 50);
    
    //floor
    auto* floor = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    floor->get_trsf()->set_translation({0,0,0});
    // floor->get_trsf()->set_rotation({0, 0, -20});
    floor->get_trsf()->set_scale({5, 1, 5});
    floor->set_meshes({slab_mesh});
    floor->set_material(lab_mat_color);
    floor->set_debug_rendering(true, {0.25, 0.65, 0.8});
    m_physics_system->add_rigid_body(new RigidBodyVolume(floor,0));

    //walls
    auto* wall = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    wall->get_trsf()->set_translation({0.2,0,0});
    wall->get_trsf()->set_scale({1, 2, 1});
    wall->get_trsf()->set_rotation({0, 0, -90});
    wall->set_meshes({slab_mesh});
    wall->set_material(lab_mat_color);
    wall->set_debug_rendering(true, {0.25, 0.65, 0.8});
    m_physics_system->add_rigid_body(new RigidBodyVolume(wall,0));


    //cube
    m_cube = new NodeGameSG(m_shaders, m_root,OBB_TYPE);
    m_cube->get_trsf()->set_translation({1,20,0});
    m_cube->get_trsf()->set_rotation({46,12,33});
//    m_cube->get_trsf()->set_uniform_scale(1/5.f);
    m_cube->set_meshes({cube_mesh2});
    m_cube->set_material(new MaterialColor(m_shaders, {0.75, 0.3, 0.95}, 50));
    m_cube->set_debug_rendering(true, {0.85, 0.5, 1});

    //ball
    auto* m_ball = new NodeGameSG(m_shaders, m_root,SphereBB_TYPE);
    m_ball->get_trsf()->set_translation({0.,20,0});
    m_ball->set_meshes({ball_mesh1});
    m_ball->set_material(new MaterialColor(m_shaders, {0.75, 0.3, 0.95}, 50));
    m_ball->set_debug_rendering(true);

////   Cube 2
//    auto* m_cube2 = new NodeGameSG(m_shaders, m_cube,OBB_TYPE);
//    m_cube2->get_trsf()->set_translation({2,2,2});
////    m_cube2->get_trsf()->set_uniform_scale(1/5.f);
//    m_cube2->set_meshes({cube_mesh2});
//    m_cube2->set_material(new MaterialColor(m_shaders, {0.85, 0.5, 0.45}, 50));

    auto* gravity_force = new GravityForce();
    auto* rbv_cube = new RigidBodyVolume(m_cube,1);
    auto* rbv_sphere = new RigidBodyVolume(m_ball,1);
    rbv_cube->add_force(gravity_force);
    rbv_sphere->add_force(gravity_force);
    m_physics_system->add_rigid_body(rbv_cube);
    m_physics_system->add_rigid_body(rbv_sphere);
//    auto* rbv_cube2 = new RigidBodyVolume(m_cube2,1000,0.01,1);
//    rbv_cube2->add_force(gravity_force);
//    m_physics_system.add_rigid_body(rbv_cube2);

    //CHARACTER
    m_character = new Character(m_shaders, m_root);
    m_cameras.push_back(m_character->get_camera());

    //SCENE ITEMS (grabbable items)
    m_items.push_back(rbv_cube);
    m_items.push_back(rbv_sphere);

    //PROJECTION
    mat4 projection_mat = perspective(radians(45.0f), 4.f / 3.0f, 0.1f, 10000.0f);
    glUniformMatrix4fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::PROJ_MAT_LOC_NAME), 1,
                       GL_FALSE, &projection_mat[0][0]);
}

std::vector<RigidBodyVolume*> LabScene::get_items(){
    return m_items;
}


RigidBodyVolume* LabScene::in_sight(){
    //RAYCAST character's sight grabbable items in scene
    glm::vec3 sight = m_character->get_sight();
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


void LabScene::update(GLFWwindow *window, float delta_time, glm::vec3 forward){
    NodeGameSG *camera_node = m_character->get_camera();
    camera_node->update_view_mat(forward);
    camera_node->update_view_pos();

    //update character sight (computed with mouse listener)
    m_character->set_sight(forward);
    
    //update ITEM IN HAND 
    if(m_character->has_item() ){
        m_character->update_item();
    }
        

    process_input(window, delta_time);
    // glm::vec3 forces = m_items[0]->get_forces();
    // std::cout<<forces[0]<<", "<<forces[1]<<", "<<forces[2]<<std::endl;
}

void LabScene::process_input(GLFWwindow *window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float camera_speed = 15.f * delta_time;
    float camera_speed_rot = 150 * delta_time;
    float cube_translate_speed = 15 * delta_time;
    
    Transform *character_trsf = m_character->get_character_node()->get_trsf();

    glm::vec3 forward_vec = m_character->get_sight();
    glm::vec3 right_vec = glm::cross(forward_vec, glm::vec3(0., 1., 0.));
    forward_vec[1] = 0.;    //disable flight

    //Camera Translation
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        glm::vec3 dir;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            dir += camera_speed * right_vec;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            dir += -camera_speed * right_vec;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            dir += -camera_speed * forward_vec;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            dir += camera_speed * forward_vec;
        }

        character_trsf->set_translation(character_trsf->get_translation() + character_trsf->apply_to_vector(dir));
    }

    if (!character_trsf->is_up_to_date()) character_trsf->compute();
    
    //Be sure to get only 1 input
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
            
            if(m_character->has_item()){

                m_character->throw_item();
            }else{

                //GRAB ITEM
                RigidBodyVolume* rbv = in_sight();
                if(rbv != 0){
                    m_character->grab_item(rbv);
                    glm::vec3 forces = rbv->get_forces();
                }
            }
        }

    }

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

    //DEBUG
    // glm::vec3 chara_pos = character_trsf->get_translation();
    // std::cout<<"CHARACTER POS: "<<chara_pos[0]<<","<<chara_pos[1]<<","<<chara_pos[2]<<std::endl;
}

Character* LabScene::get_character(){
    return m_character;
}


