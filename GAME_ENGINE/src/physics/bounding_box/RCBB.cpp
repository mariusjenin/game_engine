//
// Created by mariusjenin on 24/04/2022.
//

#include "RCBB.hpp"
#include "AABB.hpp"
#include <glm/glm.hpp>
#include <iostream>

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
