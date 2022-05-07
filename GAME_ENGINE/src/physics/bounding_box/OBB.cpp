//
// Created by mariusjenin on 24/04/2022.
//

#include <iostream>
#include "OBB.hpp"
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

// jamais utilisé: implémenté dans sphereBB
Collision OBB::get_data_collision(const SphereBB &bb) {
    return {};
}

Collision OBB::get_data_collision(const AABB &bb) {
    return {}; //TODO
}

Collision OBB::get_data_collision(const OBB &bb) {
    Collision collision;
    glm::mat3 bb_orientation = bb.get_orientation();
    std::vector<glm::vec3> test;
    test.reserve(15);
    test = {
            m_orientation[0], m_orientation[1], m_orientation[2],
            bb_orientation[0], bb_orientation[1], bb_orientation[2]
    };
    for (int i = 0; i< 3; ++i) { // Fill out rest of axis
        test[6 + i * 3 + 0] = glm::cross(test[i], test[0]);
        test[6 + i * 3 + 1] = glm::cross(test[i], test[1]);
        test[6 + i * 3 + 2] = glm::cross(test[i], test[2]);
    }
    glm::vec3* hit_normal = 0;
    bool should_flip;
    for (int i = 0; i< 15; ++i) {
        if (glm::dot(test[i],test[i])< 0.001f) {
            continue;
        }
        float depth = penetrate_depth((RCBB *) &bb, test[i], &should_flip);
        if (depth <= 0.0f) {
            return collision;
        }else if (depth < collision.depth) {
            if (should_flip) {
                test[i] = test[i] * -1.0f;
            }
            collision.depth = depth;
            hit_normal = &test[i];
        }
    }
    if (hit_normal == 0) {
        return collision;
    }

    glm::vec3 axis = glm::normalize(*hit_normal);
    std::vector<glm::vec3> c1 = get_intersections_lines(bb.to_edges());
    std::vector<glm::vec3> c2 = bb.get_intersections_lines(to_edges());

    collision.contacts.reserve(c1.size() + c2.size());
    collision.contacts.insert(collision.contacts.end(), c1.begin(), c1.end());
    collision.contacts.insert(collision.contacts.end(), c2.begin(), c2.end());

    Interval interval = get_interval(axis);
    float distance = (interval.max - interval.min)* 0.5f - collision.depth * 0.5f;
    glm::vec3 point_on_plane = m_position + axis * distance;
    size_t size_contacts_minus_one = collision.contacts.size() - 1;
    for (int i = (int)size_contacts_minus_one; i>= 0; --i) {
        glm::vec3 contact = collision.contacts[i];
        collision.contacts[i] = contact + (axis * glm::dot(axis, point_on_plane - contact));
    }
    collision.colliding = true;
    collision.normal = axis;
    return collision;
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

bool OBB::is_point_in(glm::vec3 point) const{
    glm::vec3 direction = point - m_position;
    for (int i = 0; i < 3; ++i) {
        float distance = glm::dot(direction, m_orientation[i]);
        if (distance > m_size[i]) {
            return false;
        }
        if (distance < -m_size[i]) {
            return false;
        }
    }

    return true;
}

const glm::mat3 &OBB::get_orientation() const {
    return m_orientation;
}

glm::vec3 OBB::closest_point(glm::vec3 pt) const{
    glm::vec3 result = m_position;
    glm::vec3 dir = pt - m_position;

    for(int i = 0; i < 3; i ++){
        float distance = glm::dot(dir, m_orientation[i]);
        if (distance > m_size[i]) {
            distance = m_size[i];
        }
        if (distance < -m_size[i]) {
            distance = -m_size[i];
        }

        result += (m_orientation[i] * distance);

    }

    return result;
}

float OBB::is_intersected(Ray ray){
    glm::mat3 bb_orientation = m_orientation;
    glm::vec3 sz = m_size;

    glm::vec3 p = m_position - ray.origin;
    glm::vec3 f = {
        glm::dot(bb_orientation[0], ray.direction),
        glm::dot(bb_orientation[1], ray.direction),
        glm::dot(bb_orientation[2], ray.direction),
    };

    glm::vec3 e = {
        glm::dot(bb_orientation[0], p),
        glm::dot(bb_orientation[1], p),
        glm::dot(bb_orientation[2], p),
    };

    float t[6] = {0};
    for(int i = 0; i < 3; i ++){
        if(cmp_float(f[i], 0)){
            if(-e[i] - sz[i] > 0 || -e[i] + sz[i] < 0){
                return -1.f;
            }
            f[i] = 0.000001f;
        }

        t[i * 2] = (e[i] + sz[i]) / f[i];
        t[i * 2 + 1] = (e[i] - sz[i]) / f[i];
    }

    float tmin = fmaxf(
                    fmaxf(
                        fminf(t[0], t[1]),
                        fminf(t[2], t[3])
                        ),
                    fminf(t[4], t[5])
                    );

    float tmax = fminf(
                    fminf(
                        fmaxf(t[0], t[1]),
                        fmaxf(t[2], t[3])
                        ),
                    fmaxf(t[4], t[5])
                    );
                    
    if(tmax < 0)
        return -1.f;
    
    if(tmin > tmax)
        return -1.f;

    if(tmin < 0.f)
        return tmax;
    
    return tmin;
}



