//
// Created by mariusjenin on 03/03/2022.
//

#ifndef GAME_ENGINE_VAODATAMANAGER_HPP
#define GAME_ENGINE_VAODATAMANAGER_HPP

#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

namespace shader {
    /// Manager of VBOs and EBO
    class VAODataManager {
    private:
        /**
         * Constructor of VAODataManager
         */
        VAODataManager();

    public:

        const static int ID_VERTEX_BUFFER = 0;
        const static int ID_NORMAL_BUFFER = 1;
        const static int ID_UV_BUFFER = 2;

        /**
         * Generate a new VAO
         * @param vao_id
         */
        static void generate_vao(GLuint *vao_id);

        /**
         * Generate a new VBO
         * @param bo_id
         */
        static void generate_bo(GLuint *bo_id);

        /**
         * Bind the VAO with the given id
         * @param vao_id
         */
        static void bind_vao(GLuint vao_id);

        /**
         * Enable the pointer for a VBO
         * @param index_vbo
         * @param vbo_id
         * @param size_data
         * @param normalized
         */
        static void enable_attrib_vbo(GLuint index_vbo, GLuint vbo_id, GLint size_data, GLboolean normalized);

        /**
         * Delete a BO
         * @param bo_id
         */
        static void delete_bo(GLuint bo_id);

        /**
         * Delete a VAO
         * @param vao_id
         */
        static void delete_vao(GLuint vao_id);

        /**
         * Fill a buffer with datas
         * @tparam T
         * @param buffer_type
         * @param vbo_id
         * @param datas
         */
        template<typename T>
        static void fill_bo(GLenum buffer_type, GLuint vbo_id, std::vector<T> datas);

        /**
         * Disable the pointer for a VBO
         * @param index_vbo
         */
        static void disable_attrib_vbo(GLuint index_vbo);

        /**
         * Draw the EBO with the given id
         * @param ebo_id
         * @param nb_indices
         */
        static void draw(GLuint ebo_id, long nb_indices);

    };
}

#endif //GAME_ENGINE_VAODATAMANAGER_HPP
