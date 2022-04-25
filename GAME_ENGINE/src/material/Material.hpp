//
// Created by mariusjenin on 06/03/2022.
//

#ifndef GAME_ENGINE_MATERIAL_HPP
#define GAME_ENGINE_MATERIAL_HPP

#include <GL/glew.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>
#include <iostream>
#include <src/shader/Shaders.hpp>

using namespace shader;
namespace material {
    /// Base %Material (Abstract)
    class Material {
    protected :
        float m_shininess;
        Shaders *m_shaders;

        /**
         * Constructor of a Material
         * @param shaders
         * @param shininess
         */
        explicit Material(Shaders *shaders, float shininess = 0.);

    public :
        const static int MATERIAL_TYPE_COLOR = 0;
        const static int MATERIAL_TYPE_TEXTURE = 1;

        /// Load the Material datas in the shader
        virtual void load_in_shader();
    };
}
#endif //GAME_ENGINE_MATERIAL_HPP
