//
// Created by mariusjenin on 01/03/2022.
//

#ifndef GAME_ENGINE_MESHLOADER_HPP
#define GAME_ENGINE_MESHLOADER_HPP

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <src/mesh/Mesh.hpp>

using namespace glm;
using namespace mesh;

enum NormalDirection{
    X_NORMAL_DIRECTION,
    Y_NORMAL_DIRECTION,
    Z_NORMAL_DIRECTION,
    X_INV_NORMAL_DIRECTION,
    Y_INV_NORMAL_DIRECTION,
    Z_INV_NORMAL_DIRECTION
};

/**
 * Create a plane Mesh
 * @param nb_vertex_1 first resolution
 * @param nb_vertex_2 second resolution
 * @param pos_vertex_start first vertex
 * @param pos_vertex_end last vertex
 * @param normal normal direction
 * @return plane mesh datas
 */
MeshData create_plane(
        int nb_vertex_1,
        int nb_vertex_2,
        glm::vec3 pos_vertex_start,
        glm::vec3 pos_vertex_end,
        NormalDirection normal_dir
);

/**
 * Create a sphere Mesh
 * @param radius
 * @param slices
 * @param stacks
 * @return sphere mesh datas
 */
MeshData create_sphere(
        float radius = 0.5, int slices = 32, int stacks = 16);

/**
 * Create a rectangle cuboid Mesh
 * @param size
 * @return cube mesh datas
 */
MeshData create_rectangle_cuboid(glm::vec3 size);

#endif //GAME_ENGINE_MESHLOADER_HPP
