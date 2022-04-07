//
// Created by mariusjenin on 01/03/2022.
//

#ifndef GAME_ENGINE_ROOTSG_HPP
#define GAME_ENGINE_ROOTSG_HPP


#include "NodeSG.hpp"

namespace scene_graph {
    /// Root of the %scene graph
    class RootSG : public ElementSG {
    public:
        /**
         * Constructor of the RootSG
         * @param shaders
         * @param name
         */
        explicit RootSG(Shaders *shaders, std::string name = "");

        glm::mat4 get_matrix_recursive_extern() override;

        void draw(glm::vec3 pos_camera) override;
    };
}

#endif //GAME_ENGINE_ROOTSG_HPP
