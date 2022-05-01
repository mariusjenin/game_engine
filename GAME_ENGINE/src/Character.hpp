#ifndef GAME_ENGINE_CHARACTER_HPP
#define GAME_ENGINE_CHARACTER_HPP

#include "scene_graph/NodeGameSG.hpp"

class Character {
    private:
        glm::vec3 m_sight;
        NodeGameSG* m_character_node;
        RigidBodyVolume* m_item;
        NodeGameSG* m_camera;
    public:

        Character(Shaders*, ElementSG*);

        NodeGameSG* get_character_node();


        NodeGameSG* get_camera();
        void set_camera(NodeGameSG*);

        glm::vec3 get_sight();
        void set_sight(glm::vec3);

        void grab_item(NodeGameSG);
        void throw_item();



};
#endif
