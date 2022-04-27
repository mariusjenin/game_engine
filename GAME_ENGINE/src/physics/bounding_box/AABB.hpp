//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_AABB_H
#define GAME_ENGINE_AABB_H

#include "RCBB.hpp"

namespace physics {

    namespace bounding_box {
        /// BoundingBox aligned on axis
        class AABB : public RCBB {
        public :
            /**
             * Empty Constructor of an AABB
             */
            AABB();

            /**
             * Constructor of an AABB
             */
            AABB(glm::vec3 position, glm::vec3 size);

            AABB *to_AABB() override;

            void apply_transform(glm::mat4 matrix) override;

            Collision get_data_collision(const SphereBB &bb) override;

            Collision get_data_collision(const AABB &bb) override;

            Collision get_data_collision(const OBB &bb) override;
        };
    }
}

#endif //GAME_ENGINE_AABB_H
