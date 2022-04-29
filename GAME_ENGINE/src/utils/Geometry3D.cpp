//
// Created by mariusjenin on 27/04/2022.
//

#include "Geometry3D.hpp"

using namespace utils;

// ------------------ LINE
Line::Line(glm::vec3 s, glm::vec3 e) {
    start = s;
    end = e;
}

float Line::length() const {
    return glm::distance(start,end);
}

bool Line::intersection_plane(Plane plane, glm::vec3 * intersection) const {
    glm::vec3 ab = end - start;
    float n_AB = glm::dot(plane.normal, ab);
    if (cmp_float(n_AB, 0.f)) {
        return false;
    }
    float n_A = glm::dot(plane.normal, start);
    float t = (plane.distance - n_A) / n_AB;

    if (t >= 0.0f && t <= 1.0f) {
        if (intersection != nullptr) {
            *intersection = start + ab * t;
        }
        return true;
    }

    return false;
}

// ------------------ RAY

Ray::Ray(glm::vec3 o, glm::vec3 d) {
    origin = o;
    direction = d;
}

// ------------------ PLANE

Plane::Plane(glm::vec3 n, float d) {
    normal = glm::normalize(n);
    distance = d;
}

float Plane::equation(glm::vec3 point) const {
    return glm::dot(point,normal) - distance;
}

Plane Plane::plane_from_normal_point(glm::vec3 n, glm::vec3 p) {
    glm::vec3 normal = glm::normalize(n);
    return {normal, -glm::dot(p,normal)};
}

Plane Plane::plane_from_normal_vectors(glm::vec3 p, glm::vec3 v1, glm::vec3 v2) {
    glm::vec3 normal = glm::cross(v1,v2);
    return plane_from_normal_point(normal,p);
}

Plane Plane::plane_from_points(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
    glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
    return plane_from_normal_point(normal,v1);
}

Interval::Interval(float mi, float ma) {
    min = mi;
    max = ma;
}
