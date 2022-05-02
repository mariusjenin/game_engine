//
// Created by mariusjenin on 07/04/2022.
//

#include <iostream>
#include "BoundingBox.hpp"
#include "AABB.hpp"

using namespace physics;
using namespace physics::bounding_box;

BB_TYPE BoundingBox::get_type() const {
    return m_type;
}

const glm::vec3 &BoundingBox::get_position() const {
    return m_position;
}

void BoundingBox::set_position(const glm::vec3 &position) {
    m_position = position;
}

bool BoundingBox::compute(std::vector<BoundingBox *> bbs) {
    std::vector<glm::vec3> vertices = {};
    size_t size_bbs = bbs.size();
    for(int i = 0; i < size_bbs; i++){
        std::vector<glm::vec3> verticies_2 = bbs[i]->to_vertices();
        for(auto & vert : verticies_2){
            vertices.push_back(vert);
        }
    }
    compute(vertices);
    return true;
}