//
// Created by mariusjenin on 01/03/2022.
//

#ifndef GAME_ENGINE_ELEMENTSG_HPP
#define GAME_ENGINE_ELEMENTSG_HPP


#include <map>
#include <src/shader/VAODataManager.hpp>
#include "src/utils/Transform.hpp"
#include <utility>


namespace shader {
    class Shaders;
}
using namespace shader;
namespace scene_graph {
    class NodeSG;

    /// Base Element of the scene graph (Abstract)
    class ElementSG {
    protected:
        /// Transformation applied to the ElementSG and all the children
        Transform * m_trsf;
        Shaders *m_shaders;
        std::vector<ElementSG *> m_children;
        std::map<GLuint, int> m_uniform_1i; //add others type of uniform value possible
    private:
        /**
         * Load all the uniform int datas to the shader
         */
        void load_uniforms_1i();
    public :
        /**
         * Add a child to the ElementSG
         * @param node
         */
        void add_child(NodeSG *node);

        /**
         * Add a data to be loaded to the shader at a given location
         * @param location
         * @param val
         */
        void add_uniform_1i(GLint location, int val = 0);

        /**
         * Load all the uniform datas to the shader
         */
        void load_uniforms();

        /**
         * Constructor of the ElementSG
         * @param shaders
         * @param name
         */
        explicit ElementSG(Shaders *shaders);

        /**
         * Getter of the transformation matrix of the ElementSG
         * @return trsf
         */
        Transform* get_trsf();

        /**
         * Give the matrix for an extern object (like a child) (recursive function)
         * @return matrix
         */
        virtual glm::mat4 get_matrix_recursive() = 0;

        /// Compute all the Transform list (itself and children)
        virtual void compute_trsf_scene_graph();

        /// Draw recursively the graph of the scene
        virtual void draw(glm::vec3 pos_camera);

        /// Destructor of the ElementSG
        virtual ~ElementSG();
    };
}

#endif //GAME_ENGINE_ELEMENTSG_HPP