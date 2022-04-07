//
// Created by mariusjenin on 20/03/2022.
//

#ifndef GAME_ENGINE_NODEONTOPSG_HPP
#define GAME_ENGINE_NODEONTOPSG_HPP

#include "NodeSG.hpp"

namespace scene_graph {
    /// NodeSG on top of another
    class NodeOnTopSG : public NodeSG {
    private:
        NodeSG *m_on_node;
    public:
        /**
         * Constructor of NodeOnTopSG
         * @param shaders
         * @param parent
         * @param on_node NodeSG under
         * @param name
         */
        NodeOnTopSG(Shaders *shaders, ElementSG *parent, NodeSG *on_node, std::string name = "");

        /**
         * Compute the data according to the NodeSG under
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
