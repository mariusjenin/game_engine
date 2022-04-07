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
        std::string m_name;
        /// Transformations applied to the ElementSG and all the children
        std::vector<Transform *> m_trsfs_general;
        Shaders *m_shaders;
        std::vector<ElementSG *> m_children;
        std::map<GLuint, int> m_uniform_1i; //add others type of uniform value possible
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
         * Constructor of the ElementSG
         * @param shaders
         * @param name
         */
        explicit ElementSG(Shaders *shaders, std::string name = "");

        /**
         * Getter of the name of the ElementSG
         * @return
         */
        std::string get_name();

        /**
         * Test if the name is equal to a given other one
         * @param name
         * @return is_same_name
         */
        bool is_name(const std::string &name);

        /**
         * Setter of the general Transform list
         * @param trsfs
         */
        void set_trsfs_general(std::vector<Transform *> trsfs);

        /**
         * Find recursively among the children if one as a given name
         * @param elem
         * @param name
         * @return is_found
         */
        bool find_node_by_name(ElementSG **elem, const std::string &name);

        /**
         * Give the matrix for an extern object (like a child) (recursive function)
         * @return matrix
         */
        virtual glm::mat4 get_matrix_recursive_extern() = 0;

        /// Compute all the Transform list (itself and children)
        virtual void compute_trsf_scene_graph();

        /// Draw recursively the graph of the scene
        virtual void draw(glm::vec3 pos_camera);

        /// Destructor of the ElementSG
        virtual ~ElementSG();

        /**
         * Getter of the general Transform list
         * @return trsfs
         */
        virtual std::vector<Transform *> get_trsfs_general();
    };
}

#endif //GAME_ENGINE_ELEMENTSG_HPP