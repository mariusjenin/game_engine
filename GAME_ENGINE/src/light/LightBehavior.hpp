//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_LIGHTBEHAVIOR_H
#define GAME_ENGINE_LIGHTBEHAVIOR_H

#include "LightShader.hpp"

namespace light {

    /// Behavior of a Light
    class LightBehavior {
    public:
        /**
         * Apply the behavior on the LightShader to update its data's
         * @param light_shader
         */
        virtual void apply_to(LightShader *light_shader) = 0;
    };

}


#endif //GAME_ENGINE_LIGHTBEHAVIOR_H
