
#ifndef GAME_ENGINE_LABSCENE_H
#define GAME_ENGINE_LABSCENE_H

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
    class LabScene : public Scene {
    private:
        NodeGameSG *m_cube;
    protected:
        void process_input(GLFWwindow *window, float delta_time) override;

    public:

        /**
         * Constructor of the LabScene
         * @param vertex_shader_path
         * @param fragment_shader_path
         */
        LabScene(const std::string &vertex_shader_path, const std::string &fragment_shader_path);
    };

}

#endif //GAME_ENGINE_LABSCENE_H

