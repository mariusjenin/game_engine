//
// Created by mariusjenin on 07/04/2022.
//

#include "BBFactory.hpp"
#include "AABB.hpp"
#include "OBB.hpp"
#include "SphereBB.hpp"

using namespace physics::bounding_box;

BoundingBox *BBFactory::generate_bb(BB_TYPE bb_type) {
    switch (bb_type) {
        case AABB_TYPE:
            return new AABB();
        case OBB_TYPE:
            return new OBB();
        case SPHEREBB_TYPE:
            return new SphereBB();
        default:
            return nullptr;
    }
}
