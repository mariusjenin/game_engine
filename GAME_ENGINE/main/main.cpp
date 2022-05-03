// Include standard headers
#include <cstdio>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

GLFWwindow *window;


// #include <src/scene/SceneLand.hpp>
// #include <src/scene/SolarSystem.hpp>
// #include <src/scene/BounceOBBScene.hpp>
// #include <src/scene/BounceSphereBBScene.hpp>
#include <src/scene/LabScene.hpp>

#include <src/utils/printer.hpp>

using namespace scene;

bool first_mouse = true;
float cam_yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float cam_pitch =  0.0f;
float last_x =  1024.f / 2.0;
float last_y =  748.f / 2.0;
glm::vec3 front = {0,0,-1};
float camera_speed_rot;


void process_mouse(GLFWwindow *window, double x, double y);
LabScene* lab_scene;
int main() {

    


    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    //Get the primary monitor size and pos
    int pos_monitor_x,pos_monitor_y;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorPos(monitor, &pos_monitor_x, &pos_monitor_y);

    // Open a window and create its OpenGL context
    const unsigned int WINDOW_WIDTH = 1024;
    const unsigned int WINDOW_HEIGHT = 768;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GAME ENGINE", nullptr, nullptr);

    //Get the primary monitor size and pos
    glfwSetWindowPos(window, pos_monitor_x, pos_monitor_y);

    if (window == nullptr) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, WINDOW_WIDTH / 2., WINDOW_HEIGHT / 2.);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
//    glEnable(GL_CULL_FACE);

    //CREATE THE SCENE
//    SceneLand scene = SceneLand("../shader/scene_land/vertex_shader.glsl", "../shader/scene_land/fragment_shader.glsl");
    lab_scene = new LabScene("../shader/simple_scene/vertex_shader.glsl","../shader/simple_scene/fragment_shader.glsl");
    lab_scene->setup();
    // BounceOBBScene scene = BounceOBBScene("../shader/simple_scene/vertex_shader.glsl","../shader/simple_scene/fragment_shader.glsl");
//    BounceSphereBBScene scene = BounceSphereBBScene("../shader/simple_scene/vertex_shader.glsl","../shader/simple_scene/fragment_shader.glsl");
//    SolarSystem scene = SolarSystem("../shader/solar_system/vertex_shader.glsl", "../shader/solar_system/fragment_shader.glsl");

    //MOUSE PROCESSING
    glfwSetCursorPosCallback(window, process_mouse);

    // For speed computation
    auto last_time = (float)glfwGetTime();
    float current_time;

    //Frame updates
    int frames_by_second = 144;
    float delta_time_frame_acc = 0.0f;
    float delta_time_frame_fixed = 1.0f / (float)frames_by_second;
    //Physics updates
    int update_physics_by_second = 40;
    float delta_time_physics_acc = 0.0f;
    float delta_time_physics_fixed = 1.0f / (float)fmin(frames_by_second,update_physics_by_second);


    do {
        current_time = (float)glfwGetTime();
        delta_time_frame_acc += current_time - last_time;
        delta_time_physics_acc += current_time - last_time;

        if (delta_time_frame_acc > delta_time_frame_fixed) {
            delta_time_frame_acc -= delta_time_frame_fixed;
            camera_speed_rot = 150. * delta_time_frame_fixed; 
            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //RENDER SCENE
            lab_scene->render();
            lab_scene->update(window, delta_time_frame_fixed, front);

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        if (delta_time_physics_acc > delta_time_physics_fixed) {
            delta_time_physics_acc -= delta_time_physics_fixed;
            lab_scene->update_physics(window,delta_time_physics_fixed);
        }
        last_time = current_time;

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *wdow, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void process_mouse(GLFWwindow *window, double x, double y) {
    
    if(first_mouse){
        first_mouse = false;
        last_x = x;
        last_y = y;
    }
    
    float x_offset = x - last_x;
    float y_offset = last_y - y;

    last_x = x;
    last_y = y;

    float sensivity = 0.1f;

    x_offset *= sensivity;
    y_offset *= sensivity;

    cam_yaw += x_offset;
    cam_pitch += y_offset;

    if (cam_pitch > 89.0f)
        cam_pitch = 89.0f;
    if (cam_pitch < -89.0f)
        cam_pitch = -89.0f;

    front.x = cos(glm::radians(cam_yaw)) * cos(glm::radians(cam_pitch));
    front.y = sin(glm::radians(cam_pitch));
    front.z = sin(glm::radians(cam_yaw)) * cos(glm::radians(cam_pitch));
    // std::cout<<"mouse Xpos: "<<x_offset<<", mouse Ypos: "<<y_offset<<std::endl;

    glm::vec3 y_rot(0, cam_yaw, 0);    
    glm::vec3 sight = lab_scene->get_character()->get_sight();

    // std::cout<<"CHARACTER SIGHT: "<<sight[0]<<", "<<sight[1]<<", "<<sight[2]<<std::endl;
}
