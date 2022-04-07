//
// Created by mariusjenin on 05/03/2022.
//

#ifndef GAME_ENGINE_SCENE_HPP
#define GAME_ENGINE_SCENE_HPP

#include "glm/ext.hpp"
#include "src/shader/Shaders.hpp"

#include <src/scene_graph/CameraNodeSG.hpp>
#include <src/scene_graph/RootSG.hpp>
#include <src/scene_graph/LightNodeSG.hpp>
#include <src/scene_graph/RootSG.hpp>

#include <src/light/Light.hpp>

#include <src/material/Material.hpp>

#include <src/utils/meshloader.hpp>
#include <src/utils/texture.hpp>
#include <src/utils/meshloader.hpp>
#include <src/utils/texture.hpp>

namespace scene {
    /// Base %Scene (Abstract)
    class Scene {
    protected :
        static constexpr const char *ROOT_NAME = "root";

        RootSG *m_root{};
        std::vector<NodeSG *> m_nodes;
        std::vector<CameraNodeSG *> m_cameras;
        std::vector<LightNodeSG *> m_lights;
        int m_camera_index{};
        Shaders *m_shaders{};

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
         * Update function that is called at each frame
         * @param window
         * @param delta_time
         */
        virtual void update(GLFWwindow *window, float delta_time);

        /// Draw the Scene
        virtual void draw();

        /// Destructor of the Scene
        virtual ~Scene();

    };
}

#endif //GAME_ENGINE_SCENE_HPP
