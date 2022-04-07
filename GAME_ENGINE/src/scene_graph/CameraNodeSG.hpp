//
// Created by mariusjenin on 04/03/2022.
//

#ifndef GAME_ENGINE_CAMERANODESG_HPP
#define GAME_ENGINE_CAMERANODESG_HPP


// Include GLM
#include <src/scene_graph/NodeSG.hpp>

#define CAMERA_INIT_POSITION glm::vec3(0, 0, 0)
#define CAMERA_INIT_FORWARD glm::vec3(0, 0, -1)
#define CAMERA_INIT_UP glm::vec3(0, 1, 0)

namespace scene_graph {
    /// NodeSG with a camera
    class CameraNodeSG : public NodeSG {
    public:
        /**
         * Constructor of a CameraNodeSG
         * @param shaders
         * @param parent
         * @param name
         */
        CameraNodeSG(Shaders *shaders, ElementSG *parent, std::string name = "");

        ///Update the view matrix in the shader
        void update_view_mat();

        ///Update the view position in the shader
        void update_view_pos();
    };
}

#endif //GAME_ENGINE_CAMERANODESG_HPP
