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

void BoundingBox::compute(std::vector<BoundingBox *> bbs) {
    std::vector<glm::vec3> vertices = {};
    size_t size_bbs = bbs.size();
    for(int i = 0; i < size_bbs; i++){
        AABB* aabb = bbs[i]->to_AABB();
//        std::cout << aabb->get_min()[0] << " "<< aabb->get_min()[1] << " "<< aabb->get_min()[2] << " " <<aabb->get_max()[0]
//                << " " <<aabb->get_max()[1]<< " " <<aabb->get_max()[2]<< std::endl;
        vertices.push_back(aabb->get_min());
        vertices.push_back(aabb->get_max());
    }
    compute(vertices);
}