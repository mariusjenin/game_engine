//
// Created by mariusjenin on 09/05/2022.
//

#ifndef GAME_ENGINE_SHADOWMAPPINGMANAGER_H
#define GAME_ENGINE_SHADOWMAPPINGMANAGER_H

#include "ShadersDataManager.hpp"
#include "Shaders.hpp"
#include "src/light/LightShader.hpp"
#include <iostream>

namespace shader {
    ///Manager of the Shadow Mapping Shaders
    class ShadowMapShaders : public Shaders{
    public:
        /**
         * Constructor of the ShadowMapShaders
         * @param vertex_file_path
         * @param fragment_file_path
         */
        ShadowMapShaders(const char *vertex_file_path, const char *fragment_file_path);

        void load_location() const override;
    };
}


#endif //GAME_ENGINE_SHADOWMAPPINGMANAGER_H
