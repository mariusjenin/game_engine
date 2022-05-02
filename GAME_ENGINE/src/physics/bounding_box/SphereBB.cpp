//
// Created by mariusjenin on 24/04/2022.
//

#include <iostream>
#include <src/utils/Transform.hpp>
#include "SphereBB.hpp"
#include "AABB.hpp"
#include "OBB.hpp"
#include <src/utils/printer.hpp>

using namespace physics;
using namespace physics::bounding_box;

SphereBB::SphereBB() {
    m_type = SPHEREBB_TYPE;
}

void SphereBB::compute(std::vector<glm::vec3> vertices) {
    std::pair<glm::vec3, glm::vec3> bb;
    bb.first = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    bb.second = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    glm::vec3 moy_vec = {0,0,0};
    for (auto &vertex_position: vertices) {
        moy_vec += vertex_position;
        if (vertex_position[0] < bb.first[0]) bb.first[0] = vertex_position[0];
        if (vertex_position[1] < bb.first[1]) bb.first[1] = vertex_position[1];
        if (vertex_position[2] < bb.first[2]) bb.first[2] = vertex_position[2];
        if (vertex_position[0] > bb.second[0]) bb.second[0] = vertex_position[0];
        if (vertex_position[1] > bb.second[1]) bb.second[1] = vertex_position[1];
        if (vertex_position[2] > bb.second[2]) bb.second[2] = vertex_position[2];
    }
    moy_vec/=vertices.size();

    m_position = moy_vec;

    float dist = 0;
    float dist_curr;
    for (auto &vertex_position: vertices) {
        dist_curr = glm::distance(m_position,vertex_position);
        if(dist_curr > dist){
            dist = dist_curr;
        }
    }
    m_radius = dist;
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
    Collision collision;

    glm::vec3 closest_pt = bb.closest_point(m_position);
    float dist_sq = glm::dot((closest_pt-m_position), (closest_pt-m_position));
    
    if(dist_sq > m_radius*m_radius)
        return collision;
    
    glm::vec3 normal;
    if(cmp_float(dist_sq, 0.f)){
        float len2 = glm::dot(closest_pt-bb.get_position(), closest_pt-bb.get_position());
        
        if(cmp_float(len2, 0.f))
            return collision;

        normal = glm::normalize(closest_pt - bb.get_position());
    }else{

        normal = glm::normalize(m_position - closest_pt);
    }

    glm::vec3 outside_pt = m_position - normal * m_radius;
    float dist = glm::length(closest_pt - outside_pt);
    collision.colliding = true;
    collision.contacts.push_back(closest_pt + (outside_pt - closest_pt) * 0.5f);
    collision.normal = normal;
    collision.depth = dist * 0.5f;

    return collision;
}

void SphereBB::apply_transform(glm::mat4 matrix) {

    //Decompose the matrix
    glm::mat4 t,r,s;
    Transform::matrix_to_trs(matrix,t,r,s);

    //Translate the position
    glm::vec3 translate = {t[3][0] , t[3][1] , t[3][2]};
    m_position = translate + glm::vec3( s * r * glm::vec4(m_position,1) );

    //Scale the size
    m_radius = m_radius * std::max(s[0][0],std::max(s[1][1],s[2][2]));
}

Interval SphereBB::get_interval(glm::vec3 axis) {
    float dot_pos = glm::dot(axis,m_position);
    float dot_radius = glm::dot(axis,{m_radius,m_radius,m_radius});
    float min =  dot_pos - dot_radius;
    float max =  dot_pos + dot_radius;
    return {min,max};
}

std::vector<glm::vec3> SphereBB::to_vertices() const {
    auto cos_radius = (float)sin(M_PI/4);
    glm::vec3 vec_radius = {m_radius,m_radius,m_radius};
    glm::vec3 pos_cos_add = m_position + cos_radius * vec_radius;
    glm::vec3 pos_cos_substract = m_position - cos_radius * vec_radius;
    std::vector<glm::vec3> verticies = {
            {m_position[0]+m_radius,m_position[1],m_position[2]},
            {m_position[0]-m_radius,m_position[1],m_position[2]},
            {m_position[0],m_position[1]+m_radius,m_position[2]},
            {m_position[0],m_position[1]-m_radius,m_position[2]},
            {m_position[0],m_position[1],m_position[2]+m_radius},
            {m_position[0],m_position[1],m_position[2]-m_radius},
            {pos_cos_add[0],m_position[1],m_position[2]},
            {pos_cos_substract[0],m_position[1],m_position[2]},
            {m_position[0],pos_cos_add[1],m_position[2]},
            {m_position[0],pos_cos_substract[1],m_position[2]},
            {m_position[0],m_position[1],pos_cos_add[2]},
            {m_position[0],m_position[1],pos_cos_substract[2]},
            {pos_cos_add[0],pos_cos_add[1],m_position[2]},
            {pos_cos_add[0],pos_cos_substract[1],m_position[2]},
            {pos_cos_substract[0],pos_cos_add[1],m_position[2]},
            {pos_cos_substract[0],pos_cos_substract[1],m_position[2]},
            {m_position[0],pos_cos_add[1],pos_cos_add[2]},
            {m_position[0],pos_cos_substract[1],pos_cos_add[2]},
            {m_position[0],pos_cos_add[1],pos_cos_substract[2]},
            {m_position[0],pos_cos_substract[1],pos_cos_substract[2]},
            {pos_cos_add[0],m_position[1],pos_cos_add[2]},
            {pos_cos_substract[0],m_position[1],pos_cos_add[2]},
            {pos_cos_add[0],m_position[1],pos_cos_substract[2]},
            {pos_cos_substract[0],m_position[1],pos_cos_substract[2]}
    };
    return verticies;
}

glm::vec3 SphereBB::get_tensor() {
    float fraction = (2.0f / 5.0f);
    return glm::vec3(m_radius*m_radius * fraction);
}

