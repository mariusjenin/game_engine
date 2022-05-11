
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
#include "src/game_element/Character.hpp"
#include "src/game_element/DoorElement.hpp"
#include "src/game_element/ButtonElement.hpp"

namespace scene {
    /// Scene with Collisions
    class LabScene : public Scene {
    private:
        std::vector<RigidBodyVolume*> m_items;
        Character* m_character;
        float m_timing_camera_switch;

        //helper fct
        void cubePyramid(glm::vec3, float, float);
    protected:
        void process_input(float delta_time) override;
    public:

        void setRoom(float scale, float mult, int id_tex);
        /**
         * Constructor of the LabScene
         * @param window
         * @param vertex_shader_path
         * @param fragment_shader_path
         * @param mult_physics
         */
        LabScene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path, float mult_physics = 1.0f);

        void update(float delta_time) override;

        /**
         * Get all the items grabbable
         * @return items
         */
        std::vector<RigidBodyVolume*> get_items();

        /**
         * Check whether or not a RigidBodyVolume is grabbable
         * @return in_sight
         */
        RigidBodyVolume* in_sight();
    
    };

}

#endif //GAME_ENGINE_LABSCENE_H

