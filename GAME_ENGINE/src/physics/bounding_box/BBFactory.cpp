//
// Created by mariusjenin on 07/04/2022.
//

#include "BBFactory.hpp"
#include "AABB.hpp"

using namespace physics;

BoundingBox *BBFactory::generate_bb(BB_TYPE bb_type) {
    switch (bb_type) {
        case AABB_TYPE:
            return new AABB();
        case SphereBB_TYPE:
            return new AABB();
        default:
            return nullptr;
    }
}
