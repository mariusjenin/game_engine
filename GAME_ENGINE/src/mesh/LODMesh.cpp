//
// Created by mariusjenin on 19/03/2022.
//

#include "LODMesh.hpp"

using namespace mesh;
using namespace shader;

LODMesh::LODMesh(Mesh &mesh, float min_dist, float max_dist, int min_resolution, int max_resolution,
                 int levels,int bb_type) : Mesh(mesh.get_vertex_positions(), mesh.get_triangle_indices(),
                                    mesh.get_vertex_tex_coords(),
                                    mesh.get_vertex_normals(), false,bb_type) {
    m_min_dist = min_dist;
    m_max_dist = max_dist;
    m_levels = levels;
    m_lod_meshes = {};
    float d_dist = (m_max_dist - m_min_dist) / (float) (m_levels - 1);
    float d_resol = (float) (max_resolution - min_resolution) / (float) (m_levels - 1);
    float dist = m_max_dist;
    auto resol = (float) max_resolution;
    for (
            int i = 0;
            i < m_levels;
            i++) {
        Mesh *curr_mesh = new Mesh(m_vertex_positions, m_triangle_indices, m_vertex_tex_coords, m_vertex_normals,
                                   false);
        curr_mesh->simplify((int)
                                    std::round(resol)
        );
        curr_mesh->load_bb(bb_type);
        curr_mesh->load_mesh_in_vao();
        m_lod_meshes.push_back(curr_mesh);
        dist -= d_dist;
        resol = resol - d_resol;
    }
}

void LODMesh::update_mesh(float dist_to_camera) {
    if (m_levels <= 1) {
        if (dist_to_camera > m_min_dist) {
            load_mesh_at_index(0);
        }
    } else {
        float d_dist = (m_max_dist - m_min_dist) / (float) (m_levels - 1);
        int index = std::min((int) std::floor((float) (dist_to_camera - m_min_dist) / d_dist), m_levels - 1);
        if (index > 0) {
            load_mesh_at_index(index);
        }
    }
}

LODMesh::LODMesh(const Mesh &mesh, float dist_treshold, int resolution,int bb_type) : Mesh(mesh.get_vertex_positions(),
                                                                               mesh.get_triangle_indices(),
                                                                               mesh.get_vertex_tex_coords(),
                                                                               mesh.get_vertex_normals(), false,bb_type) {
    m_min_dist = dist_treshold;
    m_max_dist = dist_treshold;
    m_levels = 1;
    m_lod_meshes = {new Mesh(m_vertex_positions, m_triangle_indices, m_vertex_tex_coords, m_vertex_normals, false)};
    m_lod_meshes.at(0)->simplify((int) std::round(resolution));
    m_lod_meshes.at(0)->load_mesh_in_vao();
}

void LODMesh::load_mesh_at_index(int index) {
    m_vertex_positions = m_lod_meshes.at(index)->get_vertex_positions();
    m_triangle_indices = m_lod_meshes.at(index)->get_triangle_indices();
    m_vertex_tex_coords = m_lod_meshes.at(index)->get_vertex_tex_coords();
    m_vertex_normals = m_lod_meshes.at(index)->get_vertex_normals();
    m_vao_id = m_lod_meshes.at(index)->get_vao_id();
    m_vbo_position_id = m_lod_meshes.at(index)->get_vbo_position_id();
    m_vbo_tex_coords_id = m_lod_meshes.at(index)->get_vbo_tex_coords_id();
    m_vbo_normals_id = m_lod_meshes.at(index)->get_vbo_normals_id();
    m_ebo_triangle_indices_id = m_lod_meshes.at(index)->get_ebo_triangle_indices_id();
    m_bb = m_lod_meshes.at(index)->get_bb();
}

LODMesh::~LODMesh() {
    for(auto & m_lod_mesh : m_lod_meshes){
        delete m_lod_mesh;
    }
}