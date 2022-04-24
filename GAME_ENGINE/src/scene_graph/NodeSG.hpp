//
// Created by marius.jenin@etu.umontpellier.fr on 17/02/2022.
//

#ifndef GAME_ENGINE_NODESG_HPP
#define GAME_ENGINE_NODESG_HPP


#include <vector>
#include <src/material/Material.hpp>
#include "src/mesh/Mesh.hpp"
#include "src/utils/Transform.hpp"
#include "ElementSG.hpp"

using namespace mesh;
using namespace material;
namespace scene_graph {
    /// Base Node of the scene graph
    class NodeSG : public ElementSG {
    private:
        Transform * m_local_trsf;
        std::vector<Mesh *> m_meshes;
        Material *m_material{};
        bool m_has_material;
        ElementSG *m_parent;
        bool m_see_both_face;
    protected:
        bool m_is_node_on_top;
    public:
        /**
         * Constructor of the NodeSG
         * @param shaders
         * @param parent parent in the scene graph
         * @param name
         */
        NodeSG(Shaders *shaders, ElementSG *parent, std::string name = "");

        /**
         * Getter of the local transformation matrix of the NodeSG
         * @return trsf
         */
        Transform* get_local_trsf();

        /**
         * Setter of the \link mesh::Mesh Mesh\endlink list of the NodeSG
         * @param meshes
         */
        void set_meshes(std::vector<Mesh *> meshes);

        /**
         * Setter of the material applied to the NodeSG
         * @param material
         */
        void set_material(Material *material);

        /**
         * Getter of the \link mesh::Mesh Mesh\endlink list of the NodeSG
         * @return meshes
         */
        const std::vector<Mesh *> &get_meshes() const;

        glm::mat4 get_matrix_recursive() override;

        /**
         * Give the matrix for the NodeSG (recursive function)
         * @return matrix
         */
        glm::mat4 get_matrix_recursive_local();

        /**
         * Gives the center of the NodSG (mean of each \link mesh::Mesh Mesh\endlink centers)
         * @param pos_camera
         * @return center
         */
        glm::vec3 get_center(glm::vec3 pos_camera);

        /**
         * Gives the aligned axis bounding box of the node (bounding box of \link mesh::Mesh Mesh\endlink bounding boxes)
         * @param pos_camera
         * @return bounding_box
         */
        std::pair<glm::vec3, glm::vec3> get_aabb(glm::vec3 pos_camera);

        void compute_trsf_scene_graph() override;

        void draw(glm::vec3 pos_camera) override;

        /**
         * Setter of the flag see_both_face (render both face of the \link mesh::Mesh Meshes\endlink))
         * @param see_both_face
         */
        void set_see_both_face(bool see_both_face);

        /**
         * Compute the position in the world of the NodeSG
         * @param v
         * @return position
         */
        glm::vec3 get_position_in_world(glm::vec3 v = {0, 0, 0});

        /**
         * Compute the distance between the bounding box and a given position
         * @param cam_position
         * @param position
         * @return distance
         */
        float get_distance_from(glm::vec3 cam_position, glm::vec3 position);

        ~NodeSG() override;
    };
}

#endif //GAME_ENGINE_NODESG_HPP
