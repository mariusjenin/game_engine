//
// Created by mariusjenin on 19/03/2022.
//

#ifndef GAME_ENGINE_LODMESH_HPP
#define GAME_ENGINE_LODMESH_HPP


#include "Mesh.hpp"

namespace mesh {
    /// Mesh with Level of Details
    class LODMesh : public Mesh {
    private:
        std::vector<Mesh *> m_lod_meshes;
        float m_min_dist;
        float m_max_dist;
        int m_levels;

        /**
         * Load the Mesh present at a given index
         * @param index
         */
        void load_mesh_at_index(int index);

    public:
        void update_mesh(float dist_to_camera) override;

        /**
         * Constructor of a LODMesh
         * @param mesh base mesh
         * @param min_dist minimum distance of the levels of details
         * @param max_dist maximum distance of the levels of details
         * @param min_resolution minimum resolution of the simplification of the levels of details
         * @param max_resolution maximum resolution of the simplification of the levels of details
         * @param levels number of levels of details
         */
        LODMesh(Mesh &mesh, float min_dist, float max_dist, int min_resolution, int max_resolution, int levels,int bb_type = BBFactory::AABB_TYPE);

        /**
         * Simple constructor of a LODMesh
         * @param mesh base mesh
         * @param dist_treshold treshold of the level of details (only one level here)
         * @param resolution resolution of the simplification of the levels of details
         */
        LODMesh(const Mesh &mesh, float dist_treshold, int resolution,int bb_type = BBFactory::AABB_TYPE);

        ~LODMesh();
    };

}
#endif //GAME_ENGINE_LODMESH_HPP
