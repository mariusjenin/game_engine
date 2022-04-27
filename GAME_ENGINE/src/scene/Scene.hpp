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
#include "src/shader/Shaders.hpp"

using namespace scene_graph;
namespace scene {
    /// Base %Scene (Abstract)
    class Scene {
    protected :
        RootSG *m_root{};
        std::vector<NodeGameSG *> m_cameras;
        std::vector<NodeGameSG *> m_lights;
        int m_camera_index{};
        Shaders *m_shaders{};
        PhysicsSystem m_physics_system;

        /**
         * Process the input of the user to have actions on the Scene
         * @param window
         * @param delta_time
         */
        virtual void process_input(GLFWwindow *window, float delta_time) = 0;

    public:
        /**
         * Constructor of the Scene with the paths to the shader files
         * @param vertex_shader_path
         * @param fragment_shader_path
         */
        Scene(const std::string &vertex_shader_path, const std::string &fragment_shader_path);

        /// Set up to load and compute datas of the Scene
        virtual void setup();

        /**
         * Getter of the shaders
         * @return
         */
        Shaders *get_shaders() const;

        /**
         * Render the Scene
         * @param window
         * @param delta_time
         */
        virtual void update(GLFWwindow *window, float delta_time);

        /**
         * Update the PhysicsSystem
         * @param window
         * @param delta_time
         */
        virtual void update_physics(GLFWwindow *window, float delta_time);

        /// Destructor of the Scene
        virtual ~Scene();

        ///Render the Scene
        void render();
    };
}

#endif //GAME_ENGINE_SCENE_HPP
