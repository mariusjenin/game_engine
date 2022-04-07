//
// Created by marius.jenin@etu.umontpellier.fr on 17/02/2022.
//

#include "Mesh.hpp"


using namespace mesh;
using namespace shader;

Mesh::Mesh(const std::vector<glm::vec3> &vp, const std::vector<unsigned short> &ti, const std::vector<glm::vec2> &vtc,
           const std::vector<glm::vec3> &vn, bool load_data_now) {
    m_vertex_positions = vp;
    m_triangle_indices = ti;
    m_vertex_tex_coords = vtc;
    m_vertex_normals = vn;
    m_center = center();
    m_bounding_box = get_bounding_box();
    if (load_data_now) load_mesh_in_vao();
    m_loaded_vao = load_data_now;
}

void Mesh::load_mesh_in_vao() {
    //Generating VAO for this Mesh and binding it
    VAODataManager::generate_vao(&m_vao_id);
    VAODataManager::bind_vao(m_vao_id);
    //Generating all the VBO
    VAODataManager::generate_bo(&m_vbo_position_id);
    VAODataManager::generate_bo(&m_vbo_tex_coords_id);
    VAODataManager::generate_bo(&m_vbo_normals_id);
    VAODataManager::generate_bo(&m_ebo_triangle_indices_id);
    //Fill the VBO data
    VAODataManager::fill_bo<glm::vec3>(GL_ARRAY_BUFFER, m_vbo_position_id, m_vertex_positions);
    VAODataManager::fill_bo<glm::vec2>(GL_ARRAY_BUFFER, m_vbo_tex_coords_id, m_vertex_tex_coords);
    VAODataManager::fill_bo<glm::vec3>(GL_ARRAY_BUFFER, m_vbo_normals_id, m_vertex_normals);
    VAODataManager::fill_bo<unsigned short int>(GL_ELEMENT_ARRAY_BUFFER, m_ebo_triangle_indices_id, m_triangle_indices);
    //Attrib Pointers
    VAODataManager::enable_attrib_vbo(VAODataManager::ID_VERTEX_BUFFER, m_vbo_position_id, 3, GL_FALSE);
    VAODataManager::enable_attrib_vbo(VAODataManager::ID_UV_BUFFER, m_vbo_tex_coords_id, 2, GL_FALSE);
    VAODataManager::enable_attrib_vbo(VAODataManager::ID_NORMAL_BUFFER, m_vbo_normals_id, 3, GL_TRUE);
}

std::pair<glm::vec3, glm::vec3> Mesh::get_bounding_box(float enlargement) {
    std::pair<glm::vec3, glm::vec3> bb;
    bb.first = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    bb.second = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (auto &vertex_position: m_vertex_positions) {
        if (vertex_position[0] < bb.first[0]) bb.first[0] = vertex_position[0];
        if (vertex_position[1] < bb.first[1]) bb.first[1] = vertex_position[1];
        if (vertex_position[2] < bb.first[2]) bb.first[2] = vertex_position[2];
        if (vertex_position[0] > bb.second[0]) bb.second[0] = vertex_position[0];
        if (vertex_position[1] > bb.second[1]) bb.second[1] = vertex_position[1];
        if (vertex_position[2] > bb.second[2]) bb.second[2] = vertex_position[2];
    }

    if (enlargement != 0.) {
        bb.first[0] -= enlargement;
        bb.first[1] -= enlargement;
        bb.first[2] -= enlargement;
        bb.second[0] += enlargement;
        bb.second[1] += enlargement;
        bb.second[2] += enlargement;
    }

    return bb;
}

void Mesh::simplify(int r, float enlargement) {
    struct TmpVertex {
        unsigned int nb_verticies;
        glm::vec3 vertex;
        glm::vec3 normal;
        glm::vec2 uv;
        std::vector<unsigned int> previous_ind_verticies;

        TmpVertex() {
            nb_verticies = 0;
            vertex = glm::vec3(0, 0, 0);
            normal = glm::vec3(0, 0, 0);
            uv = glm::vec2(0, 0);
        }
    };
    std::vector<TmpVertex> map_simplify;
    std::vector<glm::vec3> new_vertices;
    std::vector<glm::vec3> new_normals;
    std::vector<glm::vec2> new_uvs;
    std::vector<int> vertices_to_new_vertices;
    vertices_to_new_vertices.resize(m_vertex_positions.size(), -1);
    float dx, dy, dz;
    int x, y, z;

    std::pair<glm::vec3, glm::vec3> bb = get_bounding_box(enlargement);
    dx = (bb.second[0] - bb.first[0]) / (float) r;
    dy = (bb.second[1] - bb.first[1]) / (float) r;
    dz = (bb.second[2] - bb.first[2]) / (float) r;

    //INIT
    map_simplify.resize(r * r * r, TmpVertex());

    int nb_verticies = (int) m_vertex_positions.size();
    for (unsigned int i = 0; i < nb_verticies; i++) {
        x = std::floor((m_vertex_positions[i][0] - bb.first[0]) / dx);
        y = std::floor((m_vertex_positions[i][1] - bb.first[1]) / dy);
        z = std::floor((m_vertex_positions[i][2] - bb.first[2]) / dz);
        int index = x + y * r + z * r * r;
        map_simplify[index].vertex += m_vertex_positions[i];
        map_simplify[index].normal += m_vertex_normals[i];
        map_simplify[index].uv += m_vertex_tex_coords[i];
        map_simplify[index].nb_verticies++;
        map_simplify[index].previous_ind_verticies.push_back(i);
    }

    unsigned int counter_new_vertices = 0;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < r; j++) {
            for (int k = 0; k < r; k++) {
                int index = i + j * r + k * r * r;
                if (map_simplify[index].nb_verticies > 0) {
                    map_simplify[index].vertex[0] /= (float) map_simplify[index].nb_verticies;
                    map_simplify[index].vertex[1] /= (float) map_simplify[index].nb_verticies;
                    map_simplify[index].vertex[2] /= (float) map_simplify[index].nb_verticies;

                    map_simplify[index].normal[0] /= (float) map_simplify[index].nb_verticies;
                    map_simplify[index].normal[1] /= (float) map_simplify[index].nb_verticies;
                    map_simplify[index].normal[2] /= (float) map_simplify[index].nb_verticies;

                    map_simplify[index].uv[0] /= (float) map_simplify[index].nb_verticies;
                    map_simplify[index].uv[1] /= (float) map_simplify[index].nb_verticies;

                    new_vertices.push_back(map_simplify[index].vertex);
                    new_normals.push_back(map_simplify[index].normal);
                    new_uvs.push_back(map_simplify[index].uv);
                    for (unsigned int previous_ind_vertex: map_simplify[index].previous_ind_verticies) {
                        vertices_to_new_vertices[previous_ind_vertex] = (int) counter_new_vertices;
                    }
                    counter_new_vertices++;
                }
            }
        }
    }
    unsigned int triangle_indices_size = m_triangle_indices.size();

    std::vector<unsigned short> new_triangle_indices;
    for (unsigned int i = 0; i < triangle_indices_size; i += 3) {
        m_triangle_indices[i] = vertices_to_new_vertices[m_triangle_indices[i]];
        m_triangle_indices[i + 1] = vertices_to_new_vertices[m_triangle_indices[i + 1]];
        m_triangle_indices[i + 2] = vertices_to_new_vertices[m_triangle_indices[i + 2]];
        if (m_triangle_indices[i] != m_triangle_indices[i + 1] &&
            m_triangle_indices[i + 1] != m_triangle_indices[i + 2] &&
            m_triangle_indices[i] != m_triangle_indices[i + 2]) {
            new_triangle_indices.push_back(m_triangle_indices[i]);
            new_triangle_indices.push_back(m_triangle_indices[i + 1]);
            new_triangle_indices.push_back(m_triangle_indices[i + 2]);
        }
    }

    m_vertex_positions = new_vertices;
    m_vertex_normals = new_normals;
    m_vertex_tex_coords = new_uvs;
    m_triangle_indices = new_triangle_indices;
}

glm::vec3 Mesh::center() {
    glm::vec3 center = {0, 0, 0};
    for (auto &vertex_position: m_vertex_positions) {
        center[0] += vertex_position[0];
        center[1] += vertex_position[1];
        center[2] += vertex_position[2];
    }
    int nb_verticies = (int) m_vertex_positions.size();
    center[0] /= (float) nb_verticies;
    center[1] /= (float) nb_verticies;
    center[2] /= (float) nb_verticies;
    return center;
}

void Mesh::update_mesh(float dist_to_camera) {
}

const std::vector<glm::vec3> &Mesh::get_vertex_positions() const {
    return m_vertex_positions;
}

const std::vector<unsigned short int> &Mesh::get_triangle_indices() const {
    return m_triangle_indices;
}

const std::vector<glm::vec2> &Mesh::get_vertex_tex_coords() const {
    return m_vertex_tex_coords;
}

const std::vector<glm::vec3> &Mesh::get_vertex_normals() const {
    return m_vertex_normals;
}

glm::vec3 Mesh::get_center() {
    return m_center;
}

bool Mesh::get_data_at_coords(glm::vec2 pos_in_plan_xz, Transform on_top_trsf, glm::vec3 &position, glm::vec2 &uv,
                              glm::vec3 &normal) {
    struct Triangle2D {
        glm::vec2 p1, p2, p3;

        Triangle2D(glm::vec2 pt1, glm::vec2 pt2, glm::vec2 pt3) : p1(pt1), p2(pt2), p3(pt3) {}

        bool point_in_triangle(glm::vec2 pt, float &u, float &v, float &w) const {
            glm::vec2 v0 = p2 - p1;
            glm::vec2 v1 = p3 - p1;
            glm::vec2 v2 = pt - p1;
            float d00 = glm::dot(v0, v0);
            float d01 = glm::dot(v0, v1);
            float d11 = glm::dot(v1, v1);
            float d20 = glm::dot(v2, v0);
            float d21 = glm::dot(v2, v1);
            float denom = d00 * d11 - d01 * d01;
            v = (d11 * d20 - d01 * d21) / denom;
            w = (d00 * d21 - d01 * d20) / denom;
            u = 1.0f - v - w;

            return u >= 0 && v >= 0 && w >= 0;
        }
    };
    std::pair<glm::vec3, glm::vec3> bb = m_bounding_box;
    bb.first = on_top_trsf.apply_to_point(bb.first);
    bb.second = on_top_trsf.apply_to_point(bb.second);
    if (bb.first[0] > pos_in_plan_xz[0] || bb.second[0] < pos_in_plan_xz[0] ||
        bb.first[2] > pos_in_plan_xz[1] || bb.second[2] < pos_in_plan_xz[1]) {
        return false;
    }


    int size_indices = (int) m_triangle_indices.size();
    for (int i = 0; i < size_indices; i += 3) {
        glm::vec3 p3_1, p3_2, p3_3;
        glm::vec2 p2_1, p2_2, p2_3;
        p3_1 = m_vertex_positions.at(m_triangle_indices.at(i));
        p3_2 = m_vertex_positions.at(m_triangle_indices.at(i + 1));
        p3_3 = m_vertex_positions.at(m_triangle_indices.at(i + 2));
        p3_1 = on_top_trsf.apply_to_point(p3_1);
        p3_2 = on_top_trsf.apply_to_point(p3_2);
        p3_3 = on_top_trsf.apply_to_point(p3_3);
        p2_1 = {p3_1[0], p3_1[2]};
        p2_2 = {p3_2[0], p3_2[2]};
        p2_3 = {p3_3[0], p3_3[2]};
        Triangle2D t2d = Triangle2D(p2_1, p2_2, p2_3);
        float u, v, w;
        if (t2d.point_in_triangle(pos_in_plan_xz, u, v, w)) {
            position = u * m_vertex_positions.at(m_triangle_indices.at(i)) +
                       v * m_vertex_positions.at(m_triangle_indices.at(i + 1)) +
                       w * m_vertex_positions.at(m_triangle_indices.at(i + 2));
            uv = u * m_vertex_tex_coords.at(m_triangle_indices.at(i)) +
                 v * m_vertex_tex_coords.at(m_triangle_indices.at(i + 1)) +
                 w * m_vertex_tex_coords.at(m_triangle_indices.at(i + 2));
            normal = u * m_vertex_normals.at(m_triangle_indices.at(i)) +
                     v * m_vertex_normals.at(m_triangle_indices.at(i + 1)) +
                     w * m_vertex_normals.at(m_triangle_indices.at(i + 2));
            return true;
        }
    }
    return false;

}

GLuint Mesh::get_vao_id() const {
    return m_vao_id;
}


GLuint Mesh::get_vbo_position_id() const {
    return m_vbo_position_id;
}

GLuint Mesh::get_vbo_tex_coords_id() const {
    return m_vbo_tex_coords_id;
}

GLuint Mesh::get_vbo_normals_id() const {
    return m_vbo_normals_id;
}

GLuint Mesh::get_ebo_triangle_indices_id() const {
    return m_ebo_triangle_indices_id;
}

Mesh::~Mesh() {
    if (m_loaded_vao) {
        VAODataManager::disable_attrib_vbo(VAODataManager::ID_VERTEX_BUFFER);
        VAODataManager::disable_attrib_vbo(VAODataManager::ID_UV_BUFFER);
        VAODataManager::disable_attrib_vbo(VAODataManager::ID_NORMAL_BUFFER);
        VAODataManager::delete_bo(m_vbo_position_id);
        VAODataManager::delete_bo(m_vbo_tex_coords_id);
        VAODataManager::delete_bo(m_vbo_normals_id);
        VAODataManager::delete_bo(m_ebo_triangle_indices_id);
        VAODataManager::delete_vao(m_vao_id);
    }
}
