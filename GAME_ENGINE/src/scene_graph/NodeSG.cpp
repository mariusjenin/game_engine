//
// Created by marius.jenin@etu.umontpellier.fr on 17/02/2022.
//

#include "NodeSG.hpp"

#include <utility>
#include <src/physics/bounding_box/BBFactory.hpp>
#include <src/physics/bounding_box/AABB.hpp>
#include <src/utils/printer.hpp>


using namespace scene_graph;

NodeSG::NodeSG(Shaders *shaders, ElementSG *parent, std::string name)
        : ElementSG(shaders, std::move(name)) {
    m_parent = parent;
    m_parent->add_child(this);
    m_trsfs_self_after = {};
    m_trsfs_children_after = {};
    m_trsfs_self_before = {};
    m_trsfs_children_before = {};
    m_has_material = false;
    m_is_node_on_top = false;
    m_see_both_face = false;
}

glm::mat4 NodeSG::get_matrix_recursive_extern() {
    glm::mat4 mat = m_parent->get_matrix_recursive_extern();
    for (auto trsf: m_trsfs_children_after) {
        mat *= trsf->get_matrix();
    }
    for (auto trsf: m_trsfs_general) {
        mat *= trsf->get_matrix();
    }
    for (auto trsf: m_trsfs_children_before) {
        mat *= trsf->get_matrix();
    }
    return mat;
}

glm::mat4 NodeSG::get_matrix_recursive_intern() {
    glm::mat4 mat = m_parent->get_matrix_recursive_extern();
    for (auto trsf: m_trsfs_self_after) {
        mat *= trsf->get_matrix();
    }
    for (auto trsf: m_trsfs_general) {
        mat *= trsf->get_matrix();
    }
    for (auto trsf: m_trsfs_self_before) {
        mat *= trsf->get_matrix();
    }
    return mat;
}

void NodeSG::draw(glm::vec3 pos_camera) {

    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();

    if (m_see_both_face) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
    }
    //Model matrix and Normal model (if non scalar transform)
    glm::mat4 model = get_matrix_recursive_intern();
    glm::mat4 normal_model = glm::transpose(glm::inverse(model));
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::MODEL_MAT_LOC_NAME), 1, GL_FALSE,
                       &model[0][0]);
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::NORMAL_MODEL_MAT_LOC_NAME), 1, GL_FALSE,
                       &normal_model[0][0]);

    //UNIFORM 1 INT
    for (const auto &unif_1i_pair: m_uniform_1i) {
        glUniform1i((GLint) unif_1i_pair.first, unif_1i_pair.second);
    }
    if (m_has_material) {
        m_material->load_in_shader();
    }

    for (auto mesh: m_meshes) {
        mesh->update_mesh(get_distance_from(pos_camera, pos_camera));
        const std::vector<glm::vec3> &vertex_positions = mesh->get_vertex_positions();
        const std::vector<unsigned short int> &triangle_indices = mesh->get_triangle_indices();
        const std::vector<glm::vec2> &vertex_tex_coords = mesh->get_vertex_tex_coords();
        const std::vector<glm::vec3> &vertex_normals = mesh->get_vertex_normals();
        VAODataManager::bind_vao(mesh->get_vao_id());
        VAODataManager::draw(mesh->get_ebo_triangle_indices_id(), (long) triangle_indices.size());
    }
    ElementSG::draw(pos_camera);
    glUniform1i(shader_data_manager->get_location(ShadersDataManager::IS_NODE_ON_TOP_LOC_NAME), false);
}

NodeSG::~NodeSG() {
    for (auto mesh: m_meshes) {
        delete mesh;
    }
    m_meshes.clear();
    for (auto trsf: m_trsfs_self_after) {
        delete trsf;
    }
    m_trsfs_self_after.clear();
    for (auto trsf: m_trsfs_self_before) {
        delete trsf;
    }
    m_trsfs_self_before.clear();
    for (auto trsf: m_trsfs_children_after) {
        delete trsf;
    }
    m_trsfs_children_after.clear();
    for (auto trsf: m_trsfs_children_before) {
        delete trsf;
    }
    m_trsfs_children_before.clear();
    delete m_parent;
}

std::vector<Transform *> NodeSG::get_trsfs_self_after() {
    return m_trsfs_self_after;
}

std::vector<Transform *> NodeSG::get_trsfs_self_before() {
    return m_trsfs_self_before;
}

std::vector<Transform *> NodeSG::get_trsfs_children_after() {
    return m_trsfs_children_after;
}

std::vector<Transform *> NodeSG::get_trsfs_children_before() {
    return m_trsfs_children_before;
}

void NodeSG::set_trsfs_self_after(std::vector<Transform *> trsfs) {
    m_trsfs_self_after = std::move(trsfs);
}

void NodeSG::set_trsfs_self_before(std::vector<Transform *> trsfs) {
    m_trsfs_self_before = std::move(trsfs);
}

void NodeSG::set_trsfs_children_after(std::vector<Transform *> trsfs) {
    m_trsfs_children_after = std::move(trsfs);
}

void NodeSG::set_trsfs_children_before(std::vector<Transform *> trsfs) {
    m_trsfs_children_before = std::move(trsfs);
}

void NodeSG::set_meshes(std::vector<Mesh *> meshes) {
    m_meshes = std::move(meshes);
}

void NodeSG::set_material(Material *material) {
    m_material = material;
    m_has_material = true;
}

void NodeSG::set_see_both_face(bool see_both_face) {
    m_see_both_face = see_both_face;
}

glm::vec3 NodeSG::get_position_in_world(glm::vec3 center) {
    Transform trsf = Transform();
    trsf.set_matrix(get_matrix_recursive_intern());
    return trsf.apply_to_point(center);
}

void NodeSG::compute_trsf_scene_graph() {

    for (auto trsf: m_trsfs_self_before) {
        trsf->compute();
    }

    for (auto trsf: m_trsfs_self_after) {
        trsf->compute();
    }
    for (auto trsf: m_trsfs_children_before) {
        trsf->compute();
    }

    for (auto trsf: m_trsfs_children_after) {
        trsf->compute();
    }
    ElementSG::compute_trsf_scene_graph();
}

const std::vector<Mesh *> &NodeSG::get_meshes() const {
    return m_meshes;
}

glm::vec3 NodeSG::get_center(glm::vec3 pos_camera) {
    glm::vec3 center = {0, 0, 0};
    for (auto mesh: m_meshes) {
        mesh->update_mesh(get_distance_from(pos_camera, pos_camera));
        center += mesh->get_center();
    }
    center /= m_meshes.size();
    return center;
}

std::pair<glm::vec3, glm::vec3> NodeSG::get_aabb(glm::vec3 pos_camera) {
    std::pair<glm::vec3, glm::vec3> bb;
    bb.first = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    bb.second = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    Transform trsf = Transform();
    trsf.set_matrix(get_matrix_recursive_intern());
    for (auto mesh: m_meshes) {

        mesh->update_mesh(glm::distance(get_position_in_world(mesh->get_center()), pos_camera));
        std::pair<glm::vec3, glm::vec3> bb_curr;
        AABB aabb = AABB();
        aabb.compute(mesh->get_vertex_positions());
        bb_curr.first = aabb.get_min();
        bb_curr.second = aabb.get_max();
        bb_curr.first = trsf.apply_to_point(bb_curr.first);
        bb_curr.second = trsf.apply_to_point(bb_curr.second);
        for (int i = 0; i < 3; i++) {
            if (bb_curr.first[i] > bb_curr.second[i]) {
                float tmp = bb_curr.first[i];
                bb_curr.first[i] = bb_curr.second[i];
                bb_curr.second[i] = tmp;
            }
        }
        if (bb_curr.first[0] < bb.first[0]) bb.first[0] = bb_curr.first[0];
        if (bb_curr.first[1] < bb.first[1]) bb.first[1] = bb_curr.first[1];
        if (bb_curr.first[2] < bb.first[2]) bb.first[2] = bb_curr.first[2];
        if (bb_curr.second[0] > bb.second[0]) bb.second[0] = bb_curr.second[0];
        if (bb_curr.second[1] > bb.second[1]) bb.second[1] = bb_curr.second[1];
        if (bb_curr.second[2] > bb.second[2]) bb.second[2] = bb_curr.second[2];
    }

    return bb;
}

float NodeSG::get_distance_from(glm::vec3 cam_position, glm::vec3 position) {

    std::pair<glm::vec3, glm::vec3> bb = get_aabb(cam_position);
    glm::vec3 min_vals;
    float curr_val;
    for (int i = 0; i < 3; i++) {
        min_vals[i] = FLT_MAX;
        if (position[i] > bb.first[i] && position[i] < bb.second[i]) {
            min_vals[i] = 0;
        } else {
            curr_val = std::abs(bb.first[i] - position[i]);
            if (curr_val < min_vals[i]) min_vals[i] = curr_val;
            curr_val = std::abs(bb.second[i] - position[i]);
            if (curr_val < min_vals[i]) min_vals[i] = curr_val;
        }
    }
    return std::max(min_vals[0], std::max(min_vals[1], min_vals[2]));
}


