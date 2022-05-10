//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_BOUNCESPHEREBBSCENE_H
#define GAME_ENGINE_BOUNCESPHEREBBSCENE_H

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
    class BounceSphereBBScene : public Scene {
    private:
        NodeGameSG *m_ball;
        NodeGameSG *m_node_balls;
        float m_timer;
        void generate_balls(float height = 0, float radius = 1,  float dispersion = 0, int nb_balls = 10, bool texture = true);
    protected:
        void process_input(float delta_time) override;

    public:
        /**
         * Constructor of the BounceSphereBBScene
         * @param window
         * @param vertex_shader_path
         * @param fragment_shader_path
         * @param mult_physics
         */
        BounceSphereBBScene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path, float mult_physics = 1.0f);
    };

}

#endif //GAME_ENGINE_BOUNCESPHEREBBSCENE_H
