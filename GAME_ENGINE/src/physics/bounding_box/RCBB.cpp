//
// Created by mariusjenin on 24/04/2022.
//

#include "RCBB.hpp"
#include "AABB.hpp"
#include "SphereBB.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <src/physics/Collision.hpp>
#include <src/utils/printer.hpp>

using namespace physics;
using namespace physics::bounding_box;
using namespace utils;

void RCBB::compute(std::vector<glm::vec3> vertices) {
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
    m_size = (bb.second - bb.first) / 2.f;
}

AABB* RCBB::to_AABB() const{
    auto* aabb = new AABB();
    aabb->compute(to_vertices());
    return aabb;
}

std::vector<Line> RCBB::to_edges() const{
    int edges_verticies_indicies[][2] = {
            {0,4},{4,5},{5,1},{1,0},
            {2,6},{6,7},{7,3},{3,2},
            {0,2},{1,3},{4,6},{5,7},
    };
    std::vector<glm::vec3> verticies = to_vertices();
    std::vector<Line> edges = {};
    for (auto & edges_verticies_indicie : edges_verticies_indicies) {
        edges.emplace_back(Line(verticies[edges_verticies_indicie[0]], verticies[edges_verticies_indicie[1]]));
    }
    return edges;
}

std::vector<Plane> RCBB::to_planes() const{
    std::vector<glm::vec3> verticies = to_vertices();
    int planes_verticies_indicies[][3] = {
            {0,1,4},{2,3,0},{6,7,2},
            {4,5,6},{2,0,6},{1,3,5},
    };

    std::vector<Plane> planes = {};
    for (auto & planes_verticies_indicie : planes_verticies_indicies) {
        planes.push_back(Plane::plane_from_points(verticies[planes_verticies_indicie[0]], verticies[planes_verticies_indicie[1]],verticies[planes_verticies_indicie[2]]));
    }
    return planes;
}

std::vector<glm::vec3> RCBB::get_intersections_lines(std::vector<Line> lines) const{
    std::vector<glm::vec3> intersections;
    glm::vec3 intersection;
    std::vector<Plane> planes = to_planes();
    size_t size_planes = planes.size();
    size_t size_lines = lines.size();
    for (int i = 0; i<size_planes; i++) {
        for (int j = 0; j<size_lines; j++) {
            if (lines[j].intersection_plane(planes[i],&intersection)) {
                if (is_point_in(intersection)) {
                    intersections.emplace_back(intersection);
                }
            }
        }
    }
    return intersections;
}

Interval RCBB::get_interval(glm::vec3 axis) {
    std::vector<glm::vec3> vertices = to_vertices();
    float min = glm::dot(axis, vertices[0]);
    float max = min;

    for (int i = 1; i < 8; ++i) {
        float projection = glm::dot(axis, vertices[i]);
        min = (projection < min) ? projection : min;
        max = (projection > max) ? projection : max;
    }
    return {min,max};
}

float RCBB::penetrate_depth(RCBB* bb, glm::vec3 axis, bool *out_should_flip) {
    axis = glm::normalize(axis);
    Interval i1 = get_interval(axis);
    Interval i2 = bb->get_interval(axis);

    if (!((i2.min <= i1.max) && (i1.min <= i2.max))) {
        return 0.0f; // No penerattion
    }
    float len1 = i1.max - i1.min;
    float len2 = i2.max - i2.min;
    float min = fminf(i1.min, i2.min);
    float max = fmaxf(i1.max, i2.max);
    float length = max - min;
    if (out_should_flip != nullptr) {
        *out_should_flip = (i2.min < i1.min);
    }
    return (len1 + len2) - length;
}

glm::vec3 RCBB::get_tensor() {
    glm::vec3 size = m_size * 2.0f;
    float fraction = (1.0f / 12.0f);
    float x2 = size.x * size.x;
    float y2 = size.y * size.y;
    float z2 = size.z * size.z;
    return {(y2 + z2) * fraction,(x2 + z2) * fraction,(x2 + y2) * fraction};
}

Collision RCBB::get_data_collision(RCBB *bb) {
    Collision collision;
    glm::mat3 bb1_orientation = get_orientation();
    glm::mat3 bb2_orientation = bb->get_orientation();
    std::vector<glm::vec3> test;
    test.reserve(15);
    test = {
            bb1_orientation[0], bb1_orientation[1], bb1_orientation[2],
            bb2_orientation[0], bb2_orientation[1], bb2_orientation[2]
    };
    for (int i = 0; i< 3; ++i) { // Fill out rest of axis
        test[6 + i * 3 + 0] = glm::cross(test[i], test[0]);
        test[6 + i * 3 + 1] = glm::cross(test[i], test[1]);
        test[6 + i * 3 + 2] = glm::cross(test[i], test[2]);
    }
    glm::vec3* hit_normal = nullptr;
    bool should_flip;
    for (int i = 0; i< 15; ++i) {
        if (glm::dot(test[i],test[i])< 0.001f) {
            continue;
        }
        float depth = penetrate_depth((RCBB *) bb, test[i], &should_flip);
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
    if (hit_normal == nullptr) {
        return collision;
    }

    glm::vec3 axis = glm::normalize(*hit_normal);
    std::vector<glm::vec3> c1 = get_intersections_lines(bb->to_edges());
    std::vector<glm::vec3> c2 = bb->get_intersections_lines(to_edges());

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

float RCBB::is_intersected(Ray ray) {
    glm::mat3 bb_orientation = get_orientation();
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

glm::vec3 RCBB::closest_point(glm::vec3 pt) const {

    glm::vec3 result = m_position;
    glm::vec3 dir = pt - m_position;
    glm::mat3 orientation = get_orientation();

    for(int i = 0; i < 3; i ++){
        float distance = glm::dot(dir, orientation[i]);
        if (distance > m_size[i]) {
            distance = m_size[i];
        }
        if (distance < -m_size[i]) {
            distance = -m_size[i];
        }
        result += (orientation[i] * distance);
    }

    return result;
}

bool RCBB::is_point_in(glm::vec3 point) const {
    glm::mat3 orientation = get_orientation();
    glm::vec3 direction = point - m_position;
    for (int i = 0; i < 3; ++i) {
        float distance = glm::dot(direction, orientation[i]);
        if (distance > m_size[i]) {
            return false;
        }
        if (distance < -m_size[i]) {
            return false;
        }
    }

    return true;
}

Collision RCBB::get_data_collision(SphereBB *bb) {
    Collision collision;

    glm::vec3 position_sphere = bb->get_position();
    glm::vec3 closest_pt = closest_point(position_sphere);
    float dist_sq = glm::dot((closest_pt-position_sphere), (closest_pt-position_sphere));

    float radius = bb->get_radius();
    if(dist_sq > radius*radius)
        return collision;

    glm::vec3 normal;
    if(cmp_float(dist_sq, 0.f)){
        float len2 = glm::dot(closest_pt-m_position, closest_pt-m_position);

        if(cmp_float(len2, 0.f))
            return collision;

        normal = glm::normalize(closest_pt - m_position);
    }else{

        normal = glm::normalize(position_sphere - closest_pt);
    }

    glm::vec3 outside_pt = position_sphere - normal * radius;
    float dist = glm::length(closest_pt - outside_pt);
    collision.colliding = true;
    collision.contacts.push_back(closest_pt + (outside_pt - closest_pt) * 0.5f);
    collision.normal = normal;
    collision.depth = dist * 0.5f;

    return collision;
}

float RCBB::get_max_dist() {
    glm::vec3 min(m_position[0] - m_size[0], m_position[1] - m_size[1], m_position[2] - m_size[2]);
    glm::vec3 max(m_position[0] + m_size[0], m_position[1] + m_size[1], m_position[2] + m_size[2]);

    return glm::length(max - min);
}
