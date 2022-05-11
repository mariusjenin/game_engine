#ifndef GAME_ENGINE_BUTTON_ELEMENT_H
#define GAME_ENGINE_BUTTON_ELEMENT_H

#include "../scene_graph/NodeGameSG.hpp"
// #include "../utils/Geometry3D.hpp"
#include <src/physics/rigid_body_behavior/MovementBehavior.hpp>
#include <src/physics/rigid_body_behavior/MoveDoorBehavior.hpp>
#include <src/physics/rigid_body_behavior/SwitchColorBehavior.hpp>
#include "src/material/MaterialTexture.hpp"
#include "src/material/MaterialColor.hpp"
#include "DoorElement.hpp"

class ButtonElement : public NodeGameSG {

private:
    RigidBodyVolume *m_top;
    std::vector<DoorElement *> m_linked = {};
    // RigidBodyVolume* m_bottom;
public:
    /**
     * Constructor of the ButtonElement
     * @param parent
     * @param bb_type
     */
    ButtonElement(ElementSG *parent, BB_TYPE bb_type, DoorElement * = nullptr);

    /**
     * Add all the RigidBodyBehavior
     * @param mult
     * @param rbv
     */
    void add_behavior(float mult, std::vector<RigidBodyVolume *> rbv);

    /**
     * Link to a door
     */
    void link_door(DoorElement *);

    /**
     * Getter the RigidBodyVolume of the button
     * @return rigid_body
     */
    RigidBodyVolume *get_rigid_body();


};

#endif