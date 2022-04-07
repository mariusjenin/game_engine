//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_SIMPLESCENE_H
#define GAME_ENGINE_SIMPLESCENE_H




#include "Scene.hpp"
#include "../mesh/LODMesh.hpp"
#include "../material/MaterialTexture.hpp"
#include "../material/MaterialColor.hpp"
#include "../light/DirectionLight.hpp"
#include "../utils/objloader.hpp"

namespace scene {
    /// Simple scene
    class SimpleScene : public Scene {
    private:
        NodeSG *m_ball;
    protected:
        void process_input(GLFWwindow *window, float delta_time) override;

    public:

        /**
         * Constructor of the SimpleScene
         * @param vertex_shader_path
         * @param fragment_shader_path
         */
        SimpleScene(const std::string &vertex_shader_path, const std::string &fragment_shader_path);

        ~SimpleScene() override;
    };

}

#endif //GAME_ENGINE_SIMPLESCENE_H
