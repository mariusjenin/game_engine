//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_BBFACTORY_H
#define GAME_ENGINE_BBFACTORY_H


#include "BoundingBox.hpp"

class BBFactory {
public:
    static BoundingBox* generate_bb(BB_TYPE bb_type);
};


#endif //GAME_ENGINE_BBFACTORY_H
