//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_SIMPLESCENE_H
#define GAME_ENGINE_SIMPLESCENE_H

#include "Scene.hpp"
#include "src/mesh/LODMesh.hpp"
#include "src/material/MaterialTexture.hpp"
#include "src/material/MaterialColor.hpp"
#include "src/light/DirectionLight.hpp"
#include "src/utils/objloader.hpp"
#include <src/light/PositionLight.hpp>
#include <src/scene_graph/NodeOnTopSG.hpp>
#include <src/light/SpotLight.hpp>

namespace scene {
    /// Simple scene
    class SimpleScene : public Scene {
    private:
        NodeGameSG *m_ball;
    protected:
        void process_input(GLFWwindow *window, float delta_time) override;

    public:

        /**
         * Constructor of the SimpleScene
         * @param vertex_shader_path
         * @param fragment_shader_path
         */
        SimpleScene(const std::string &vertex_shader_path, const std::string &fragment_shader_path);
    };

}

#endif //GAME_ENGINE_SIMPLESCENE_H
