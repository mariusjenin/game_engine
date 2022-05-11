#ifndef GAME_ENGINE_DOOR_ELEMENT_H
#define GAME_ENGINE_DOOR_ELEMENT_H

#include "../scene_graph/NodeGameSG.hpp"
// #include "../utils/Geometry3D.hpp"
#include <src/physics/rigid_body_behavior/MovementBehavior.hpp>
#include "src/material/MaterialTexture.hpp"
#include "src/material/MaterialColor.hpp"


class DoorElement : public NodeGameSG {

private:
    RigidBodyVolume *m_left;
    RigidBodyVolume *m_right;

    std::vector<glm::vec3> m_open_pos;
    std::vector<glm::vec3> m_closed_pos;
public:
    /**
     * Constructor of the DoorElement
     * @param parent
     * @param bb_typeint
     * @param id_left_texture
     * @param id_right_texture
     */
    DoorElement(ElementSG *parent, BB_TYPE bb_typeint, int id_left_texture, int id_right_texture);

    /**
     * Add all the RigidBodyBehavior
     * @param mult
     */
    void add_behavior(float mult);

    /**
     * Open the doors
     */
    void open();

    /**
     * Close the doors
     */
    void close();

    /**
     * Getter of the left door RigidBodyVolume
     * @return rigid body
     */
    RigidBodyVolume *get_left_rigid_body();

    /**
     * Getter of the right door RigidBodyVolume
     * @return rigid body
     */
    RigidBodyVolume *get_right_rigid_body();

};

#endif