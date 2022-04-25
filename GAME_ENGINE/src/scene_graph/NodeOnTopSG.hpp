//
// Created by mariusjenin on 20/03/2022.
//

#ifndef GAME_ENGINE_NODEONTOPSG_HPP
#define GAME_ENGINE_NODEONTOPSG_HPP

#include "NodeGameSG.hpp"

namespace scene_graph {
    /// NodeGameSG on top of another
    class NodeOnTopSG : public NodeGameSG {
    private:
        NodeGameSG *m_on_node;
    public:
        /**
         * Constructor of NodeOnTopSG
         * @param shaders
         * @param parent
         * @param on_node NodeGameSG under
         * @param name
         */
        NodeOnTopSG(Shaders *shaders, ElementSG *parent, NodeGameSG *on_node, BB_TYPE bb_type = AABB_TYPE);

        /**
         * Compute the data according to the NodeGameSG under
         * @param pos_camera
         * @param position
         * @param uv
         * @param normal
         * @param height_adjustement
         * @return found
         */
        bool
        get_data_on(glm::vec3 pos_camera, glm::vec3 &position, glm::vec2 &uv, glm::vec3 &normal,
                    float &height_adjustement);

        void draw(glm::vec3 pos_camera) override;
    };
}

#endif //GAME_ENGINE_NODEONTOPSG_HPP
