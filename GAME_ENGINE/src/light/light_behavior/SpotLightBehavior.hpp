//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_SPOTLIGHTBEHAVIOR_H
#define GAME_ENGINE_SPOTLIGHTBEHAVIOR_H


#include <src/shader/ShadowMap.hpp>
#include "src/light/light_behavior/LightBehavior.hpp"
#include "src/light/Light.hpp"

namespace light {

    namespace behavior {
        /// Behavior of a SpotLight
        class SpotLightBehavior : public LightBehavior {
        private:
            float m_inner_cut_off;
            float m_outer_cut_off; // if inner == outer then no smooth edge
            float m_cut_off_angle; // if inner == outer then no smooth edge
            float m_z_near;
            float m_z_far;
            float m_bias;
            GLuint m_id_texture_shadow_map;
            ShadowMap * m_shadow_map;

        public:
            /**
             * Constructor of a SpotLightBehavior with the inner, the outer cut off and the id texture for the shadow map
             * @param id_texture_shadow_map
             * @param icoa
             * @param ocoa
             * @param resol
             * @param z_near
             * @param z_far
             * @param bias
             */
            explicit SpotLightBehavior(GLuint id_texture_shadow_map, float icoa = 20.f, float ocoa = 25.f, int resol = 1000, float z_near=1.0f, float z_far = 1000.f, float bias = 0.001f);

            void apply_to(LightInfo *light_shader, glm::mat4 model_mat) override;
        };
    }
}

#endif //GAME_ENGINE_SPOTLIGHTBEHAVIOR_H
