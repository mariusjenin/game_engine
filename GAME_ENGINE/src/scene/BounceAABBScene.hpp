//
// Created by mariusjenin on 08/05/2022.
//

#ifndef GAME_ENGINE_BOUNCEAABBSCENE_H
#define GAME_ENGINE_BOUNCEAABBSCENE_H

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
    /// Scene with Collisions
    class BounceAABBScene : public Scene {
    private:
        NodeGameSG *m_cube;
    protected:
        void process_input(float delta_time) override;

    public:
        /**
         * Constructor of the BounceAABBScene
         * @param window
         * @param vertex_shader_path
         * @param fragment_shader_path
         * @param mult_physics
         */
        BounceAABBScene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path, float mult_physics = 1.0f);
    };

}

#endif //GAME_ENGINE_BOUNCEAABBSCENE_H
