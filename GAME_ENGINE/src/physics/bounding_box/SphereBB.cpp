//
// Created by mariusjenin on 24/04/2022.
//

#include <iostream>
#include <src/utils/Transform.hpp>
#include "SphereBB.hpp"
#include "AABB.hpp"

using namespace physics;
using namespace physics::bounding_box;

SphereBB::SphereBB() {
    m_type = SphereBB_TYPE;
}

void SphereBB::compute(std::vector<glm::vec3> vertices) {
    std::pair<glm::vec3, glm::vec3> bb;
    bb.first = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    bb.second = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (auto &vertex_position: vertices) {
        if (vertex_position[0] < bb.first[0]) bb.first[0] = vertex_position[0];
        if (vertex_position[1] < bb.first[1]) bb.first[1] = vertex_position[1];
        if (vertex_position[2] < bb.first[2]) bb.first[2] = vertex_position[2];
        if (vertex_position[0] > bb.second[0]) bb.second[0] = vertex_position[0];
        if (vertex_position[1] > bb.second[1]) bb.second[1] = vertex_position[1];
        if (vertex_position[2] > bb.second[2]) bb.second[2] = vertex_position[2];
    }

    m_position = (bb.first + bb.second) / 2.f;
    glm::vec3 radius = bb.second - m_position;
    m_radius = std::max(radius.x, std::max(radius.y, radius.z));
}

float SphereBB::get_radius() const {
    return m_radius;
}

AABB * SphereBB::to_AABB() const {
    return new AABB(m_position,{m_radius,m_radius,m_radius});
}

Collision SphereBB::get_data_collision(const SphereBB &bb) {
    Collision collision;

    float sum_radius = this->get_radius() + bb.get_radius();
    glm::vec3 dist = bb.get_position() - this->get_position();

    if ((glm::length(dist) * glm::length(dist) - sum_radius * sum_radius) > 0.f || glm::length(dist) == 0.f)
        return collision;                  // no collision

    dist = glm::normalize(dist);

    collision.colliding = true;
    collision.normal = dist;
    collision.depth = fabsf(glm::length(dist) - sum_radius) * 0.5f;

    float dist_to_inter = this->get_radius() - collision.depth;
    glm::vec3 contact_pt = this->get_position() + dist * dist_to_inter;

    collision.contacts.push_back(contact_pt);

    return collision;
}

Collision SphereBB::get_data_collision(const AABB &bb) {
    return {}; //TODO
}

Collision SphereBB::get_data_collision(const OBB &bb) {
    return {}; //TODO
}

void SphereBB::apply_transform(glm::mat4 matrix) {

    glm::mat4 t,r,s;
    Transform::matrix_to_trs(matrix,t,r,s);

//    std::cout << t[3][0] << " "<< t[3][1] << " "<< t[3][2] << std::endl;
//    std::cout << r[0][0] << " "<< r[1][1] << " "<< r[2][2] << std::endl;
//    std::cout << s[0][0] << " "<< s[1][1] << " "<< s[2][2] << " \n"<< std::endl;

    //Translate the position
    m_position = {t[3][0] , t[3][1] , t[3][2]};
    //Scale the size
     m_radius = m_radius * std::max(s[0][0],std::max(s[1][1],s[2][2]));
//    m_size[0] *= scale[0];
//    m_size[1] *= scale[1];
//    m_size[2] *= scale[2];

    //Rotate the orientation matrix
//    Transform trsf_orientation = Transform({},rotation);
//    trsf_orientation.compute();
    // m_orientation = m_orientation * glm::mat3(r);
//    //Decompose the matrix
//    Transform trsf = Transform(matrix);
//    glm::vec3 translation = trsf.get_translation();
//    glm::vec3 scale = trsf.get_scale();
//
//    //Translate the position
//    m_position = m_position + trsf.get_translation();
//    //Scale the size
//    m_radius *= std::max(scale[0],std::max(scale[1],scale[2]));
}

Interval SphereBB::get_interval(glm::vec3 axis) {
    float dot_pos = glm::dot(axis,m_position);
    float dot_radius = glm::dot(axis,{m_radius,m_radius,m_radius});
    float min =  dot_pos - dot_radius;
    float max =  dot_pos + dot_radius;
    return {min,max};
}

std::vector<glm::vec3> SphereBB::to_vertices() const {
    return to_AABB()->to_vertices();
}

