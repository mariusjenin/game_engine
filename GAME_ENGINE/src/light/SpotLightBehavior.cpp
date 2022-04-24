//
// Created by mariusjenin on 24/04/2022.
//

#include "SpotLightBehavior.hpp"

using namespace light;

SpotLightBehavior::SpotLightBehavior(float icoa, float ocoa) {
    m_inner_cut_off = icoa;
    m_outer_cut_off = ocoa;
}

void SpotLightBehavior::apply_to(LightShader *light_shader) {
    light_shader->inner_cut_off = m_inner_cut_off;
    light_shader->outer_cut_off = m_outer_cut_off;
}