#ifndef GAME_ENGINE_DOOR_ELEMENT_H
#define GAME_ENGINE_DOOR_ELEMENT_H

#include "../scene_graph/NodeGameSG.hpp"
// #include "../utils/Geometry3D.hpp"
#include <src/physics/rigid_body_behavior/MovementBehavior.hpp>
#include "src/material/MaterialTexture.hpp"
#include "src/material/MaterialColor.hpp"


class DoorElement : public NodeGameSG{

    private:
        RigidBodyVolume* m_left;
        RigidBodyVolume* m_right;

        std::vector<glm::vec3> m_open_pos;
        std::vector<glm::vec3> m_closed_pos;
    public:

        DoorElement(Shaders *shaders, ElementSG *parent, BB_TYPE bb_type);
        
        void add_behavior(float mult);

        void open();
        void close();

        RigidBodyVolume* get_left();
        RigidBodyVolume* get_right();


};
#endif