//
// Created by mariusjenin on 01/03/2022.
//

#include "ElementSG.hpp"
#include "NodeSG.hpp"

#include <utility>

#include <src/shader/Shaders.hpp>

using namespace scene_graph;

ElementSG::ElementSG(Shaders *shaders) {
    m_shaders = shaders;
    m_children = {};
    m_trsf = new Transform();
}

void ElementSG::add_child(NodeSG *node) {
    m_children.push_back(node);
}

void ElementSG::draw(glm::vec3 pos_camera) {
    for (auto &m_child: m_children) {
        m_child->draw(pos_camera);
    }
}

ElementSG::~ElementSG() {
    for (auto m_child: m_children) {
        delete m_child;
    }
    m_children.clear();
    delete m_trsf;
    m_uniform_1i.clear();
}

void ElementSG::add_uniform_1i(GLint location, int val) {
    m_uniform_1i[location] = val;
}

void ElementSG::compute_trsf_scene_graph() {
    m_trsf->compute();

    for (auto child: m_children) {
        child->compute_trsf_scene_graph();
    }
}

Transform *ElementSG::get_trsf() {
    return m_trsf;
}

void ElementSG::load_uniforms_1i() {
    for (const auto &unif_1i_pair: m_uniform_1i) {
        glUniform1i((GLint) unif_1i_pair.first, unif_1i_pair.second);
    }
}

void ElementSG::load_uniforms() {
    load_uniforms_1i();
}
