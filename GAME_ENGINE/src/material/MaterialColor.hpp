//
// Created by mariusjenin on 06/03/2022.
//

#ifndef GAME_ENGINE_MATERIALCOLOR_HPP
#define GAME_ENGINE_MATERIALCOLOR_HPP


#include "Material.hpp"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace material {
    /// Material with colors for the ambient, diffuse and specular components
    class MaterialColor : public Material {
    private:
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
    public:
        /**
         * Constructor of a MaterialColor
         * @param shaders
         * @param ambient
         * @param diffuse
         * @param specular
         * @param shininess
         */
        explicit MaterialColor(glm::vec3 ambient = {0, 0, 0}, glm::vec3 diffuse = {0, 0,
                                                                                                     0},
                               glm::vec3 specular = {
                                       0, 0, 0}, float shininess = 1.);

        /**
         * Constructor of a MaterialColor that have the same color for the ambient, diffuse and specular components
         * @param shaders
         * @param color
         * @param shininess
         */
        explicit MaterialColor(glm::vec3 color = {0, 0, 0}, float shininess = 1.);

        void load_in_shader(Shaders *shaders) override;
    };
}
#endif //GAME_ENGINE_MATERIALCOLOR_HPP
