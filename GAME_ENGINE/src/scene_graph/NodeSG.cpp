//
// Created by marius.jenin@etu.umontpellier.fr on 17/02/2022.
//

#include "NodeSG.hpp"

#include <utility>
#include <src/physics/bounding_box/BBFactory.hpp>
#include <src/physics/bounding_box/AABB.hpp>
#include <src/utils/printer.hpp>


using namespace scene_graph;

NodeSG::NodeSG(Shaders *shaders, ElementSG *parent)
        : ElementSG(shaders) {
    m_parent = parent;
    m_parent->add_child(this);
    m_local_trsf = new Transform();
}

glm::mat4 NodeSG::get_matrix_recursive(bool inverse) {
    if(inverse) {
        return m_trsf->get_inverse() * m_parent->get_matrix_recursive(inverse);
    }else{
        return m_parent->get_matrix_recursive(inverse) * m_trsf->get_matrix();
    }
}

glm::mat4 NodeSG::get_matrix_recursive_local(bool inverse) {
    if(inverse){
        return m_local_trsf->get_inverse() * m_trsf->get_inverse() * m_parent->get_matrix_recursive(inverse);
    } else {
        return m_parent->get_matrix_recursive(inverse) * m_trsf->get_matrix() * m_local_trsf->get_matrix();
    }
}

NodeSG::~NodeSG() {
    delete m_local_trsf;

    delete m_parent;
}

glm::vec3 NodeSG::get_position_in_world(glm::vec3 center) {
    Transform trsf = Transform();
    trsf.set_matrix(get_matrix_recursive_local());
    return trsf.apply_to_point(center);
}

void NodeSG::compute_trsf_scene_graph() {

    m_local_trsf->compute();

    ElementSG::compute_trsf_scene_graph();
}

Transform *NodeSG::get_local_trsf() {
    return m_local_trsf;
}

void NodeSG::load_model_matrices() {
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();

    //Model matrix and Normal model (if non scalar transform)
    glm::mat4 model = get_matrix_recursive_local();
    glm::mat4 normal_model = glm::transpose(glm::inverse(model));
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::MODEL_MAT_LOC_NAME), 1, GL_FALSE,
                       &model[0][0]);
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::NORMAL_MODEL_MAT_LOC_NAME), 1, GL_FALSE,
                       &normal_model[0][0]);
}


