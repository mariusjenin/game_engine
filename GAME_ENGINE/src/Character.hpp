#ifndef GAME_ENGINE_CHARACTER_HPP
#define GAME_ENGINE_CHARACTER_HPP

// #include "scene_graph/NodeGameSG.hpp"
#include "scene_graph/RootSG.hpp"
#include "physics/force/GravityForce.hpp"
#include "src/utils/meshloader.hpp"
#include "src/material/MaterialColor.hpp"


class Character {
    private:
        glm::vec3 m_sight;
        RigidBodyVolume* m_body;
        RigidBodyVolume* m_item;
        NodeGameSG* m_camera;
        
    public:

        //Scene root to cut childs from character.
        ElementSG* m_scene_root;

        Character(Shaders*, ElementSG*);

        NodeGameSG* get_character_node();
        RigidBodyVolume* get_body();

        NodeGameSG* get_camera();
        void set_camera(NodeGameSG*);

        glm::vec3 get_sight();
        void set_sight(glm::vec3);

        void grab_item(RigidBodyVolume*, float = 9.f);
        void throw_item();

        void jump();

        bool has_item();
        RigidBodyVolume* get_item();

        void update_item();

};
#endif
