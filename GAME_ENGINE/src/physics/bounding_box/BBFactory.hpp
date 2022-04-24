//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_BBFACTORY_H
#define GAME_ENGINE_BBFACTORY_H


#include "BoundingBox.hpp"
namespace physics {
    class BBFactory {
    public:
        /**
         * Create a BoundingBox according to the type given
         * @param bb_type
         * @return bounding box
         */
        static BoundingBox *generate_bb(BB_TYPE bb_type);
    };

}
#endif //GAME_ENGINE_BBFACTORY_H
