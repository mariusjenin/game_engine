//
// Created by mariusjenin on 01/03/2022.
//

#ifndef GAME_ENGINE_ELEMENTSG_HPP
#define GAME_ENGINE_ELEMENTSG_HPP


#include <map>
#include <src/shader/VAODataManager.hpp>
#include <src/shader/MainShaders.hpp>
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
        std::vector<NodeSG *> m_children;
        bool m_children_dirty;
        std::map<GLuint, int> m_uniform_1i; //add others type of uniform value possible
    private:
        /**
         * Load all the uniform int datas to the shader
         */
        void load_uniforms_1i();
    public :

        /**
         * Getter of whether or not the ElementSG is a node of the scene graph that is usable like a node of the game (NodeGameSG)
         * @return is_node_game
         */
        virtual bool is_node_game();

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
         */
        explicit ElementSG();

        /**
         * Getter of the transformation matrix of the ElementSG
         * @return trsf
         */
        Transform* get_trsf();


        /**
         * Getter of all the children NodeSG
         * @return children
         */
        std::vector<NodeSG *> get_children();

        /**
         * Remove a child from the list of NodeSG at a given position
         * @param i
         */
        void remove_child_at(int i);

        /**
         * Clear the list of NodeSG
         */
        void clear_children();

        /**
         * Getter of whether or not the ElementSG has children
         * @return has children
         */
        bool has_children() const;

        /**
         * Give the matrix for an extern object (like a child) (recursive function)
         * @return matrix
         */
        virtual glm::mat4 get_matrix_recursive(TransformDirty* dirty, bool inverse) = 0;

        /**
         * Reset all the TransformDirty of the Transform of this node and children recursively
         * @param dirty
         */
        virtual void reset_trsf_dirty(bool dirty);

        /**
         * Reset the children dirty of the this node and children recursively
         * @param dirty
         */
        virtual void reset_children_dirty(bool dirty);

        /// Compute all the Transform list (itself and children)
        virtual void compute_trsf_scene_graph();

        /**
         * Draw recursively the graph of the scene
         * @param shaders
         * @param pos_camera
         * @param allow_debug
         */
        virtual void draw(Shaders *shaders, glm::vec3 pos_camera, bool allow_debug);

        /// Destructor of the ElementSG
        virtual ~ElementSG();
    };
}

#endif //GAME_ENGINE_ELEMENTSG_HPP