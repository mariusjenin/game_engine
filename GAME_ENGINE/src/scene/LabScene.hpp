
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
#include "src/Character.hpp"

namespace scene {
    /// Scene with Collisions
    class LabScene : public Scene {
    private:
        NodeGameSG* m_cube;
        std::vector<RigidBodyVolume*> m_items;
        Character* m_character;
    protected:
        void process_input(GLFWwindow *window, float delta_time) override;
    public:

        /**
         * Constructor of the LabScene
         * @param vertex_shader_path
         * @param fragment_shader_path
         */
        LabScene(const std::string &vertex_shader_path, const std::string &fragment_shader_path);
        
        void update(GLFWwindow *window, float delta_time);
        
        Character* get_character();
        std::vector<RigidBodyVolume*> get_items();
        RigidBodyVolume* in_sight();
    
    };

}

#endif //GAME_ENGINE_LABSCENE_H

