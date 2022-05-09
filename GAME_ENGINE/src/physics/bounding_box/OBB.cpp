//
// Created by mariusjenin on 24/04/2022.
//

#include <iostream>
#include "OBB.hpp"
#include "AABB.hpp"
#include "SphereBB.hpp"
#include "src/physics/Collision.hpp"
#include <src/utils/printer.hpp>
#include <src/utils/Transform.hpp>

using namespace physics;
using namespace physics::bounding_box;

OBB::OBB() {
    m_type = OBB_TYPE;
    m_orientation = glm::mat3(1);
}

void OBB::compute(std::vector<glm::vec3> vertices) {
    RCBB::compute(vertices);
    m_orientation = glm::mat3(1);
}

std::vector<glm::vec3> OBB::to_vertices() const{
    std::vector<glm::vec3> vecs_add = {m_size.x * m_orientation[0],m_size.y* m_orientation[1],m_size.z* m_orientation[2]};
    std::vector<glm::vec3> vecs_substract = {-m_size.x* m_orientation[0],-m_size.y* m_orientation[1],-m_size.z* m_orientation[2]};
    return {
            m_position+vecs_add[0]+vecs_add[1]+vecs_add[2],
            m_position+vecs_add[0]+vecs_add[1]+vecs_substract[2],
            m_position+vecs_add[0]+vecs_substract[1]+vecs_add[2],
            m_position+vecs_add[0]+vecs_substract[1]+vecs_substract[2],
            m_position+vecs_substract[0]+vecs_add[1]+vecs_add[2],
            m_position+vecs_substract[0]+vecs_add[1]+vecs_substract[2],
            m_position+vecs_substract[0]+vecs_substract[1]+vecs_add[2],
            m_position+vecs_substract[0]+vecs_substract[1]+vecs_substract[2]
    };
}

void OBB::apply_transform(glm::mat4 matrix) {
    //Decompose the matrix
    glm::mat4 t,r,s;
    Transform::matrix_to_trs(matrix,t,r,s);

    //Translate the position
    glm::vec3 translate = {t[3][0] , t[3][1] , t[3][2]};
    m_position = translate + glm::vec3( s * r *glm::vec4(m_position,0) );

    //Scale the size
    m_size = glm::vec3(s * glm::vec4(m_size,0));

    //Rotate the orientation matrix
    m_orientation = glm::mat3(r) * m_orientation;
}

glm::mat3 OBB::get_orientation() const {
    return m_orientation;
}



