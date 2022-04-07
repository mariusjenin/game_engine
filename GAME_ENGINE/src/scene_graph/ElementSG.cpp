//
// Created by mariusjenin on 01/03/2022.
//

#include "ElementSG.hpp"
#include "NodeSG.hpp"

#include <utility>
#include <src/shader/ShadersDataManager.hpp>

#include <src/shader/Shaders.hpp>

using namespace scene_graph;

ElementSG::ElementSG(Shaders *shaders, std::string name) {
    m_shaders = shaders;
    m_children = {};
    m_name = std::move(name);
    m_trsfs_general = {};
}

std::vector<Transform *> ElementSG::get_trsfs_general() {
    return m_trsfs_general;
}

void ElementSG::add_child(NodeSG *node) {
    m_children.push_back(node);
}

bool ElementSG::find_node_by_name(ElementSG **elem, const std::string &name) {
    for (auto m_child: m_children) {
        if (m_child->is_name(name)) {
            *elem = m_child;
            return true;
        }
    }
    //Try recursively
    for (auto m_child: m_children) {
        if (m_child->find_node_by_name(elem, name)) {
            return true;
        }
    }
    return false;
}

std::string ElementSG::get_name() {
    return m_name;
}

bool ElementSG::is_name(const std::string &name) {
    return name == m_name;
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
    for (auto m_trsf: m_trsfs_general) {
        delete m_trsf;
    }
    m_trsfs_general.clear();
    m_uniform_1i.clear();
}

void ElementSG::add_uniform_1i(GLint location, int val) {
    m_uniform_1i[location] = val;
}

void ElementSG::set_trsfs_general(std::vector<Transform *> trsfs) {
    m_trsfs_general = std::move(trsfs);
}

void ElementSG::compute_trsf_scene_graph() {

    for (auto trsf: m_trsfs_general) {
        trsf->compute();
    }

    for (auto child: m_children) {
        child->compute_trsf_scene_graph();
    }
}
