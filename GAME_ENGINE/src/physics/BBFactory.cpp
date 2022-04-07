//
// Created by mariusjenin on 07/04/2022.
//

#include "BBFactory.hpp"
#include "AABB.hpp"

BoundingBox* BBFactory::generate_bb(int bb_type) {
    switch (bb_type) {
        case AABB_TYPE:
            return new AABB();
        default:
            return nullptr;
    }
}
