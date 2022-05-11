//
// Created by mariusjenin on 06/03/2022.
//

#ifndef GAME_ENGINE_MATERIALTEXTURE_HPP
#define GAME_ENGINE_MATERIALTEXTURE_HPP

#include "Material.hpp"
#include <src/utils/texture.hpp>

namespace material {
    /// Material with texture for the diffuse and specular components
    class MaterialTexture : public Material {
    private:
        int m_id_diffuse_texture;
        int m_id_specular_texture;
    public:
        /**
         * Constructor of a textured Material with the paths to the texture files to has to be loaded
         * @param texture_manager
         * @param path_diffuse_texture
         * @param path_specular_texture
         * @param shininess
         */
        MaterialTexture(TextureManager* texture_manager, const std::string &path_diffuse_texture,
                        const std::string &path_specular_texture, float shininess = 0.);

        /**
         * Constructor of a textured Material with the ids of the texture files already loaded
         * @param id_diffuse_texture
         * @param id_specular_texture
         * @param shininess
         */
        MaterialTexture(int id_diffuse_texture, int id_specular_texture, float shininess = 0.);

        /**
         * Constructor of a textured Material with the id of the texture file already loaded for the both components diffuse and specular
         * @param id_texture
         * @param shininess
         */
        explicit MaterialTexture(int id_texture, float shininess = 0.);

        void load_in_shader(Shaders *shaders) override;

    };
}
#endif //GAME_ENGINE_MATERIALTEXTURE_HPP
