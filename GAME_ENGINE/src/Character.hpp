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
        glm::vec3 m_sight;
        RigidBodyVolume* m_body;
        RigidBodyVolume* m_item;

        //Camera can rotate separately from body
        NodeGameSG* m_camera;
        float m_power;
        
        PhysicsSystem* m_physics;
    public:

        //Class to compute character's sight with mouse motion.
        MouseView* m_mouse_view = nullptr;

        //Scene root to cut childs from character.
        ElementSG* m_scene_root;

        Character(Shaders*, ElementSG*, PhysicsSystem*);

        NodeGameSG* get_character_node();
        RigidBodyVolume* get_body();

        NodeGameSG* get_camera();
        void set_camera(NodeGameSG*);

        PhysicsSystem* get_physics();
        void set_physics(PhysicsSystem*);

        glm::vec3 get_sight();
        
        void grab_item(RigidBodyVolume*, float = 9.f);
        void accumulate_power();
        void throw_item();

        bool has_item();
        RigidBodyVolume* get_item();

        void update_item();

        void jump();

};
#endif
