//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_LIGHTBEHAVIOR_H
#define GAME_ENGINE_LIGHTBEHAVIOR_H

#include "src/light/LightShader.hpp"
#include "src/scene_graph/NodeGameSG.hpp"

namespace scene_graph{
    class NodeGameSG;
}

namespace light {
    namespace behavior {

        /// Behavior of a Light (Abstract)
        class LightBehavior {
        public:

            /**
             * Apply the behavior on the LightShader to resolve its data's
             * @param light_info
             * @param model_mat
             */
            virtual void apply_to(LightInfo *light_info, glm::mat4 model_mat) = 0;
        };
    }
}


#endif //GAME_ENGINE_LIGHTBEHAVIOR_H
