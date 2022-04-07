//
// Created by mariusjenin on 20/03/2022.
//

#include "NodeOnTopSG.hpp"

using namespace scene_graph;

NodeOnTopSG::NodeOnTopSG(Shaders *shaders, ElementSG *parent, NodeSG *on_node, std::string name) : NodeSG(shaders,
                                                                                                          parent,
                                                                                                          std::move(
                                                                                                                  name)) {
    m_on_node = on_node;
    m_is_node_on_top = true;
}

bool NodeOnTopSG::get_data_on(glm::vec3 pos_camera, glm::vec3 &position, glm::vec2 &uv, glm::vec3 &normal,
                              float &height_adjustement) {

    std::vector<Mesh *> meshes = m_on_node->get_meshes();
    std::pair<glm::vec3, glm::vec3> bb = get_aabb(pos_camera);
    glm::vec3 center = get_center(pos_camera);
    Transform trsf = Transform();
    trsf.set_matrix(get_matrix_recursive_intern());
    center = trsf.apply_to_point(center);
    Transform trsf_on_node = Transform();
    trsf_on_node.set_matrix(m_on_node->get_matrix_recursive_intern());
    for (Mesh *mesh: meshes) {
        mesh->update_mesh(get_distance_from(pos_camera, pos_camera));
        if (mesh->get_data_at_coords(glm::vec2(center[0], center[2]), trsf_on_node, position, uv, normal)) {
            height_adjustement = center[1] - bb.first[1];
            return true;
        }
    }
    return false;
}

void NodeOnTopSG::draw(glm::vec3 pos_camera) {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    float height_adjustement;
    bool is_on_top_bounds = get_data_on(pos_camera, position, uv, normal, height_adjustement);
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
    glUniform1f(shader_data_manager->get_location(ShadersDataManager::ON_TOP_HEIGHT_ADJUSTMENT_LOC_NAME),
                height_adjustement);
    glUniform3fv(shader_data_manager->get_location(ShadersDataManager::ON_TOP_POSITION_LOC_NAME), 1, &position[0]);
    glUniform2fv(shader_data_manager->get_location(ShadersDataManager::ON_TOP_UV_LOC_NAME), 1, &uv[0]);
    glUniform3fv(shader_data_manager->get_location(ShadersDataManager::ON_TOP_NORMAL_LOC_NAME), 1, &normal[0]);
    glUniform1i(shader_data_manager->get_location(ShadersDataManager::IS_NODE_ON_TOP_LOC_NAME), is_on_top_bounds);
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::ON_TOP_MODEL_LOC_NAME), 1, GL_FALSE,
                       &m_on_node->get_matrix_recursive_intern()[0][0]);
    NodeSG::draw(pos_camera);
}

