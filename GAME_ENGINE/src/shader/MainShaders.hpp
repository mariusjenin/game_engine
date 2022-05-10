//
// Created by mariusjenin on 09/05/2022.
//

#ifndef GAME_ENGINE_MAINSHADERS_H
#define GAME_ENGINE_MAINSHADERS_H

#include "Shaders.hpp"
#include "ShadowMapShaders.hpp"

namespace shader{
    class MainShaders : public Shaders{
    private:
        ShadowMapShaders* m_shadow_map_shaders;
    public:
        /**
         * Constructor of the MainShaders
         * @param vertex_file_path
         * @param fragment_file_path
         */
        MainShaders(const char *vertex_file_path, const char *fragment_file_path);

        void load_location() const override;

        /**
         * Getter of the Shaders of the Shadow Map
         * @return shadow_map_shaders
         */
        ShadowMapShaders* get_shadow_map_shaders() const;
    };
}


#endif //GAME_ENGINE_MAINSHADERS_H
