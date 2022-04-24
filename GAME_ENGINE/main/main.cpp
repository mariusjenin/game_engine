// Include standard headers
#include <cstdio>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

GLFWwindow *window;


#include <src/scene/SceneLand.hpp>
#include <src/scene/SolarSystem.hpp>
#include <src/scene/SimpleScene.hpp>

using namespace scene;
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

    // Open a window and create its OpenGL context
    const unsigned int SCR_WIDTH = 1024;
    const unsigned int SCR_HEIGHT = 768;
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TP1 - GLFW", nullptr, nullptr);
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
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, SCR_WIDTH / 2., SCR_HEIGHT / 2.);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
//    glEnable(GL_CULL_FACE);

    //CREATE THE SCENE
//    SceneLand scene = SceneLand("../shader/scene_land/vertex_shader.glsl", "../shader/scene_land/fragment_shader.glsl");
//    SimpleScene scene = SimpleScene("../shader/scene_land/vertex_shader.glsl", "../shader/scene_land/fragment_shader.glsl");
    SolarSystem scene = SolarSystem("../shader/solar_system/vertex_shader.glsl", "../shader/solar_system/fragment_shader.glsl");
    scene.setup();
    Shaders *shaders = scene.get_shaders();
    GLuint program_id = shaders->get_program_id();

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    float delta_time = 0.0f; // time between current frame and last frame
    float last_frame = 0.0f;
    do {
        // Measure speed
        // per-frame time logic
        // --------------------
        auto currentFrame = (float) glfwGetTime();
        delta_time = currentFrame - last_frame;
        last_frame = currentFrame;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //UPDATE SCENE (MODIFICATION WITH ANIMATION + WITH INPUTS)
        scene.update(window, delta_time);
        scene.draw();

//        VAODataManager::disable_attrib_vertex_arrays();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VAO VBO
//    vbo_manager->delete_buffers();
    glDeleteProgram(program_id);

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
