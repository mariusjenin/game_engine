//
// Created by marius.jenin@etu.umontpellier.fr on 17/02/2022.
//

#ifndef GAME_ENGINE_MESH_HPP
#define GAME_ENGINE_MESH_HPP

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <GL/glew.h>

#include <vector>
#include <cmath>
#include <src/utils/Transform.hpp>
#include <src/shader/VAODataManager.hpp>
#include <src/physics/bounding_box/BBFactory.hpp>
#include <src/physics/bounding_box/BoundingBox.hpp>

using namespace physics;
namespace mesh {
    /// Datas of a Mesh
    struct MeshData{
        std::vector<glm::vec3> vertex_positions;
        std::vector<unsigned short int> triangle_indices;
        std::vector<glm::vec2> vertex_tex_coords;
        std::vector<glm::vec3> vertex_normals;

        /**
         * Constructor of MeshData
         * @param vp
         * @param ti
         * @param vtc
         * @param vn
         */
        MeshData(std::vector<glm::vec3> vp,std::vector<unsigned short int> ti,std::vector<glm::vec2> vtc,std::vector<glm::vec3> vn);
    };

    /// Base %Mesh
    class Mesh {
    protected:
        GLuint m_vbo_position_id{}, m_vbo_tex_coords_id{}, m_vbo_normals_id{}, m_ebo_triangle_indices_id{};
        bool m_loaded_vao;

        std::vector<glm::vec3> m_vertex_positions;
        std::vector<unsigned short int> m_triangle_indices;
        std::vector<glm::vec2> m_vertex_tex_coords;
        std::vector<glm::vec3> m_vertex_normals;

        BoundingBox* m_bb;
        glm::vec3 m_center;

        GLuint m_vao_id{};
    public:
        /**
         * Constructor of a Mesh with the vertices position, texture coordinates, normals and triangle indices
         * @param vp postions
         * @param ti triangles indices
         * @param vtc vertex texture coordinates
         * @param vn vertex normals
         */
        Mesh(const std::vector<glm::vec3> &vp, const std::vector<unsigned short> &ti, const std::vector<glm::vec2> &vtc,
             const std::vector<glm::vec3> &vn = *new std::vector<glm::vec3>(), bool load_data_now = true, BB_TYPE bb_type = AABB_TYPE);

        /**
         * Constructor of a Mesh with MeshData
         * @param md
         * @param load_data_now
         * @param bb_type
         */
        Mesh(const MeshData& md, bool load_data_now = true, BB_TYPE bb_type = AABB_TYPE);

        /// Load Mesh datas in his VAO
        void load_mesh_in_vao();

        /**
         * Update the datas of the Mesh
         * @param dist_to_camera
         */
        virtual void update_mesh(float dist_to_camera);

        /**
         * Getter of the vertex positions
         * @return vertex_positions
         */
        const std::vector<glm::vec3> &get_vertex_positions() const;

        /**
         * Getter of the triangle indices
         * @return triangle_indices
         */
        const std::vector<unsigned short int> &get_triangle_indices() const;

        /**
         * Getter of the vertex texture coords
         * @return texture_coords
         */
        const std::vector<glm::vec2> &get_vertex_tex_coords() const;

        /**
         * Getter of the vertex normals
         * @return vertex_normals
         */
        const std::vector<glm::vec3> &get_vertex_normals() const;

        /**
         * Getter of the center of the Mesh
         * @return center
         */
        glm::vec3 get_center();

        /**
         * Gives the data at a given xz position
         * @param pos_in_plan_xz
         * @param on_top_trsf
         * @param position
         * @param uv
         * @param normal
         * @return data_found
         */
        bool get_data_at_coords(glm::vec2 pos_in_plan_xz, Transform on_top_trsf, glm::vec3 &position, glm::vec2 &uv,
                                glm::vec3 &normal);

        /**
         * Simplify the Mesh with a grid approach (with a resolution) and a Mean Metric Error
         * @param r
         * @param enlargement
         */
        void simplify(int r, float enlargement = 0.001);

        /**
         * Getter of the bounding box of the Mesh
         * @param enlargement
         * @return bb
         */
        BoundingBox* get_bb();

        /**
         * Load the Bounding box of the Mesh
         * @param bb_type
         */
        void load_bb(BB_TYPE bb_type);

        /**
         * Getter of the VAO id
         * @return m_vao_id
         */
        GLuint get_vao_id() const;

        /**
         * Getter of the VBO of position id
         * @return vbo_position_id
         */
        GLuint get_vbo_position_id() const;

        /**
         * Getter of the VBO of texture coords id
         * @return vbo_tex_coords_id
         */
        GLuint get_vbo_tex_coords_id() const;

        /**
         * Getter of the VBO of normals id
         * @return vbo_normals_id
         */
        GLuint get_vbo_normals_id() const;

        /**
         * Getter of the EBO of triangle indices id
         * @return ebo_triangle_indices_id
         */
        GLuint get_ebo_triangle_indices_id() const;

        /**
         * Destructor of the Mesh
         */
        ~Mesh();

    private :


        /**
         * Compute the center of the Mesh
         * @return center
         */
        glm::vec3 center();


    };
}

#endif //GAME_ENGINE_MESH_HPP
