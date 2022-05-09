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
             * Constructor by copy
             */
            AABB(AABB const &aabb);

            /**
             * Constructor of an AABB
             */
            AABB(glm::vec3 position, glm::vec3 size);

            /**
             * Getter of the minimal value of the AABB
             * @return min
             */
            glm::vec3 get_min();

            /**
             * Getter of the maximal value of the AABB
             * @return max
             */
            glm::vec3 get_max();

            AABB *to_AABB() const override;

            void apply_transform(glm::mat4 matrix) override;

            std::vector<glm::vec3> to_vertices() const override;

            glm::mat3 get_orientation() const override;
        };
    }
}

#endif //GAME_ENGINE_AABB_H
