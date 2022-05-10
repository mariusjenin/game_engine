#ifndef GAME_ENGINE_CHARACTER_HPP
#define GAME_ENGINE_CHARACTER_HPP

// #include "scene_graph/NodeGameSG.hpp"
#include "scene_graph/RootSG.hpp"
#include "physics/force/GravityForce.hpp"
#include "src/utils/meshloader.hpp"
#include "src/material/MaterialColor.hpp"
#include "MouseView.hpp"
#include "src/shader/MainShaders.hpp"

class Character {
private:
    constexpr static float HEIGHT_JUMP = 25.f;
    constexpr static float POWER_INIT = 5.f;
    constexpr static float INCREASE_POWER = 1.5f;
    RigidBodyVolume *m_body;
    RigidBodyVolume *m_item;

    //Camera can rotate separately from body
    NodeGameSG *m_camera;
    float m_power;

    PhysicsSystem *m_physics;
    double m_act_timestamp;

    bool m_has_item;

    //Class to compute character's sight with mouse motion.
    MouseView *m_mouse_view = nullptr;

    //Scene root to cut childs from character.
    ElementSG *m_scene_root;
public:

    /**
     * Getter of the MouseView
     * @return mouse view
     */
    MouseView* get_mouse_view()const;

    /**
     * Setter of the MouseView
     * @param mouse_view
     */
    void set_mouse_view(MouseView* mouse_view) ;

    /**
     * Constructor of a Character
     * @param shaders
     * @param node
     * @param ps
     */
    Character(MainShaders * shaders, ElementSG * node, PhysicsSystem *ps);

    /**
     * Getter of the body node of the Charachter
     * @return body node
     */
    NodeGameSG *get_character_node();

    /**
     * Getter of the body
     * @return body
     */
    RigidBodyVolume *get_body();

    /**
     * Getter of the camera
     */
    NodeGameSG *get_camera();

    /**
     * Getter of the sight of the Character
     * @return sight
     */
    glm::vec3 get_sight();

    /**
     * Grab the RigidBodyVolume given
     * @param rbv
     * @param ts
     * @param action_area
     */
    void grab_item(RigidBodyVolume * rbv,  double ts, float action_area = 9.f);

    /**
     * Accumulate power to launch item
     */
    void accumulate_power();

    /**
     * Throw the item handed
     * @param ts
     */
    void throw_item(double ts);

    /**
     * Getter of the item handed
     * @return
     */
    RigidBodyVolume *get_item();

    /**
     * Getter of whether or not the Character has an item
     * @return has item
     */
    bool has_item();

    /**
     * Getter of whether or not the Character can interact
     * @param timestamp
     * @return can_interact
     */
    bool can_interact(double timestamp) const;

    /**
     * Make the Character jump
     */
    void jump();

};

#endif
