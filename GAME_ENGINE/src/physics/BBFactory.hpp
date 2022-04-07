//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_BBFACTORY_H
#define GAME_ENGINE_BBFACTORY_H


#include "BoundingBox.hpp"

class BBFactory {public:
    const static int AABB_TYPE = 0;

    static BoundingBox* generate_bb(int bb_type);
};


#endif //GAME_ENGINE_BBFACTORY_H
