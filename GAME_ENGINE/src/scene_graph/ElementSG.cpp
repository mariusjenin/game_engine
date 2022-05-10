//
// Created by mariusjenin on 01/03/2022.
//

#include "ElementSG.hpp"
#include "NodeSG.hpp"

#include <utility>

#include <src/shader/Shaders.hpp>

using namespace scene_graph;

ElementSG::ElementSG() {
    m_children = {};
    m_children_dirty = true;
    m_trsf = new Transform();
}

void ElementSG::add_child(NodeSG *node) {
    m_children.push_back(node);
    m_children_dirty = true;
}

void ElementSG::draw(Shaders *shaders, glm::vec3 pos_camera, bool allow_debug) {
    for (auto &child: m_children) {
        child->draw(shaders,pos_camera, allow_debug);
    }
}

ElementSG::~ElementSG() {
    for (auto child: m_children) {
        delete child;
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

bool ElementSG::is_node_game() {
    return false;
}

std::vector<NodeSG *> ElementSG::get_children() {
    return m_children;
}


void ElementSG::remove_child_at(int i) {
    m_children.erase(m_children.begin()+i);
    m_children_dirty = true;
}

void ElementSG::clear_children() {
    m_children.clear();
    m_children_dirty = true;
}

void ElementSG::reset_trsf_dirty(bool dirty) {
    m_trsf->is_dirty()->reset(dirty);
    for(NodeSG* node:m_children){
        node->reset_trsf_dirty(dirty);
    }
}

void ElementSG::reset_children_dirty(bool dirty) {
    m_children_dirty = false;
    for(NodeSG* node:m_children){
        node->reset_children_dirty(dirty);
    }
}

bool ElementSG::has_children() const {
    return !m_children.empty();
}
