//
// Created by mariusjenin on 24/04/2022.
//

#include "PhysicsGeometry.hpp"

using namespace physics;

//Collision PhysicsGeometry::get_data_collision(const RCBB &bb1, const RCBB &bb2){
//    //TODO
//}

Collision PhysicsGeometry::get_data_collision(const SphereBB &bb1, const SphereBB &bb2) {
    Collision collision;

    float sum_radius = bb1.get_radius() + bb2.get_radius();
    glm::vec3 dist = bb2.get_position() - bb1.get_position();

    if ((glm::length(dist) * glm::length(dist) - sum_radius * sum_radius) > 0.f || glm::length(dist) == 0.f)
        return collision;                  // pas de collision

    dist = glm::normalize(dist);

    collision.colliding = true;
    collision.normal = dist;
    collision.depth = fabsf(glm::length(dist) - sum_radius) * 0.5f;

    float dist_to_inter = bb1.get_radius() - collision.depth;
    glm::vec3 contact_pt = bb1.get_position() + dist * dist_to_inter;

    collision.contacts.push_back(contact_pt);

    return collision;
}
