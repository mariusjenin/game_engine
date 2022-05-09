#ifndef GAME_ENGINE_BUTTON_ELEMENT_H
#define GAME_ENGINE_BUTTON_ELEMENT_H

#include "../scene_graph/NodeGameSG.hpp"
// #include "../utils/Geometry3D.hpp"
#include <src/physics/rigid_body_behavior/MovementBehavior.hpp>
#include <src/physics/rigid_body_behavior/MoveDoorBehavior.hpp>
#include "src/material/MaterialTexture.hpp"
#include "src/material/MaterialColor.hpp"
#include "DoorElement.hpp"

class ButtonElement : public NodeGameSG{

    private:
        RigidBodyVolume* m_top;
        std::vector<DoorElement*> m_linked = {};
        // RigidBodyVolume* m_bottom;
    public:
        ButtonElement(Shaders *shaders, ElementSG *parent, BB_TYPE bb_type, DoorElement* = nullptr);
        void add_behavior(float mult);

        void link_door(DoorElement*);

        RigidBodyVolume* get_top();


};
#endif