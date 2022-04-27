//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_OBB_H
#define GAME_ENGINE_OBB_H


#include "RCBB.hpp"
namespace physics {

    namespace bounding_box {
        /// BoundingBox oriented
        class OBB : public RCBB {
        public:
            /**
             * Constructor of an OBB
             */
            OBB();

            void apply_transform(glm::mat4 matrix) override;

            Collision get_data_collision(const SphereBB &bb) override;

            Collision get_data_collision(const AABB &bb) override;

            Collision get_data_collision(const OBB &bb) override;
        };
    }
}
#endif //GAME_ENGINE_OBB_H
