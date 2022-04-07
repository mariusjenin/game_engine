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
/**
 * Create a plane Mesh
 * @param nb_vertex_1 first resolution
 * @param nb_vertex_2 second resolution
 * @param pos_vertex_start first vertex
 * @param pos_vertex_end last vertex
 * @param normal normal direction
 * @return plane
 */
Mesh *create_plane(
        int nb_vertex_1,
        int nb_vertex_2,
        glm::vec3 pos_vertex_start,
        glm::vec3 pos_vertex_end,
        glm::vec3 normal
);

/**
 * Create a sphere Mesh
 * @param radius
 * @param slices
 * @param stacks
 * @return sphere
 */
Mesh *create_sphere(
        float radius = 0.5, int slices = 32, int stacks = 16);

#endif //GAME_ENGINE_MESHLOADER_HPP
