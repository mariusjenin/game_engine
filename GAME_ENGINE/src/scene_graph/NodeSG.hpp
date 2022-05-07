//
// Created by marius.jenin@etu.umontpellier.fr on 17/02/2022.
//

#ifndef GAME_ENGINE_NODESG_HPP
#define GAME_ENGINE_NODESG_HPP


#include <vector>
#include "ElementSG.hpp"
#include "src/utils/Transform.hpp"
#include "src/shader/Shaders.hpp"

namespace scene_graph {
    class ElementSG;
    /// Base Node of the scene graph (Abstract)
    class NodeSG : public ElementSG {
    private:
        ElementSG *m_parent;
    protected:
        /**
         * Constructor of the NodeSG
         * @param shaders
         * @param parent parent in the scene graph
         * @param name
         */
        NodeSG(Shaders *shaders, ElementSG *parent);

        Transform *m_local_trsf;
    public:

        /**
         * Load the Model matrices of the NodeSG
         */
        void load_model_matrices();

        /**
         * Getter of the local transformation matrix of the NodeSG
         * @return trsf
         */
        Transform *get_local_trsf();


        glm::mat4 get_matrix_recursive(TransformDirty* dirty, bool inverse) override;

        /**
         * Give the matrix for the NodeSG (recursive function)
         * @return matrix
         */
        glm::mat4 get_matrix_recursive_local(TransformDirty* dirty = nullptr, bool inverse = false);


        void compute_trsf_scene_graph() override;


        /**
         * Compute the position in the world of the NodeSG
         * @param v
         * @return position
         */
        glm::vec3 get_position_in_world(glm::vec3 v = {0, 0, 0});

        void set_parent(ElementSG*);
        void reset_trsf_dirty(bool dirty) override;

        ~NodeSG() override;
    };
}

#endif //GAME_ENGINE_NODESG_HPP
