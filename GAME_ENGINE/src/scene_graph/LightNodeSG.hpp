//
// Created by mariusjenin on 05/03/2022.
//

#ifndef GAME_ENGINE_LIGHTNODESG_HPP
#define GAME_ENGINE_LIGHTNODESG_HPP


#include <src/light/Light.hpp>
#include "NodeSG.hpp"

using namespace light;
namespace scene_graph {
    /// NodeSG with a light
    class LightNodeSG : public NodeSG {
    private :
        Light *m_light;
    public:
        /**
         * Constructor of LightNodeSG
         * @param shaders
         * @param parent
         * @param light
         * @param name
         */
        explicit LightNodeSG(Shaders *shaders, ElementSG *parent, Light *light, std::string name = "");

        /**
         * Give the \link light::LightShader LightShader\endlink that correspond to the \link light::Light Light\endlink
         * @return light_shader
         */
        LightShader generate_light_struct();
    };
}

#endif //GAME_ENGINE_LIGHTNODESG_HPP
