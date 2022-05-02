#ifndef GAME_ENGINE_CHARACTER_HPP
#define GAME_ENGINE_CHARACTER_HPP

// #include "scene_graph/NodeGameSG.hpp"
#include "scene_graph/RootSG.hpp"

#include "physics/force/GravityForce.hpp"

class Character {
    private:
        glm::vec3 m_sight;
        NodeGameSG* m_character_node;
        RigidBodyVolume* m_item;
        NodeGameSG* m_camera;
        
    public:
        //Scene root to cut childs from character.
        ElementSG* m_scene_root;

        Character(Shaders*, ElementSG*);

        NodeGameSG* get_character_node();

        NodeGameSG* get_camera();
        void set_camera(NodeGameSG*);

        glm::vec3 get_sight();
        void set_sight(glm::vec3);

        void grab_item(RigidBodyVolume*, float = 4.f);
        void throw_item();

        bool has_item();
        RigidBodyVolume* get_item();

        void update_item();

};
#endif
