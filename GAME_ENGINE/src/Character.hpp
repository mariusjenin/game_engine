#ifndef GAME_ENGINE_CHARACTER_HPP
#define GAME_ENGINE_CHARACTER_HPP

// #include "scene_graph/NodeGameSG.hpp"
#include "scene_graph/RootSG.hpp"
#include "physics/force/GravityForce.hpp"
#include "src/utils/meshloader.hpp"
#include "src/material/MaterialColor.hpp"
#include "MouseView.hpp"

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

public:
    bool has_item;

    //Class to compute character's sight with mouse motion.
    MouseView *m_mouse_view = nullptr;

    //Scene root to cut childs from character.
    ElementSG *m_scene_root;

    Character(Shaders *, ElementSG *, PhysicsSystem *);

    NodeGameSG *get_character_node();

    RigidBodyVolume *get_body();

    NodeGameSG *get_camera();

    void set_camera(NodeGameSG *);

    PhysicsSystem *get_physics();

    void set_physics(PhysicsSystem *);

    glm::vec3 get_sight();

    void grab_item(RigidBodyVolume *, double, float = 9.f);

    void accumulate_power();

    void throw_item(double);

    // bool has_item();
    RigidBodyVolume *get_item();

    bool can_interact(double) const;

    void jump();

};

#endif
