//
// Created by mariusjenin on 05/03/2022.
//

#ifndef GAME_ENGINE_SCENE_HPP
#define GAME_ENGINE_SCENE_HPP

#include "glm/ext.hpp"
#include <src/scene_graph/RootSG.hpp>
#include <src/scene_graph/NodeGameSG.hpp>
#include <src/light/Light.hpp>
#include <src/material/Material.hpp>
#include <src/utils/meshloader.hpp>
#include <src/utils/texture.hpp>
#include <src/utils/meshloader.hpp>
#include <src/physics/PhysicsSystem.hpp>
#include "src/shader/MainShaders.hpp"

using namespace scene_graph;
namespace scene {
    /// Base %Scene (Abstract)
    class Scene {
    protected :
        float m_fovy{};
        float m_z_near{};
        float m_z_far{};
        GLFWwindow * m_window;
        RootSG *m_root{};
        std::vector<NodeGameSG *> m_cameras;
        std::vector<NodeGameSG *> m_lights;
        int m_camera_index{};
        MainShaders *m_shaders{};
        PhysicsSystem* m_physics_system{};

        /**
         * Process the input of the user to have actions on the Scene
         * @param delta_time
         */
        virtual void process_input(float delta_time) = 0;

        /**
         * Load the lights in the shaders
         */
        void load_lights();

        /**
         * Load the Projection Matrix
         */
        void load_projection_matrix();

        /**
         * Adapt the Viewport to the window
         */
        void adapt_viewport();
    public:
        static const int NB_MAX_LIGHTS = 10;
        /**
         * Constructor of the Scene with the paths to the shader files
         * @param window
         * @param vertex_shader_path
         * @param fragment_shader_path
         * @param mult_physics
         */
        Scene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path, float mult_physics = 1.0f);

        /// Set up to load and compute datas of the Scene
        virtual void setup();

        /**
         * Getter of the shaders
         * @return
         */
        Shaders *get_shaders() const;

        /**
         * Render the Scene
         * @param delta_time
         */
        virtual void update(float delta_time);

        /**
         * Update the PhysicsSystem
         * @param delta_time
         */
        virtual void update_physics(float delta_time);

        /**
         * Destructor of the Scene
         */
        virtual ~Scene();

        /**
         * Render the Scene
         * @param allow_debug
         * @param other_shaders
         */
        void render(bool allow_debug = true, Shaders* shaders = nullptr);
    };
}

#endif //GAME_ENGINE_SCENE_HPP
