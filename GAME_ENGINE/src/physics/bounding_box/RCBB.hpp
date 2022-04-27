//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_RCBB_H
#define GAME_ENGINE_RCBB_H

#include "BoundingBox.hpp"
#include <cfloat>
namespace physics {

    namespace bounding_box {
        class AABB;

        /// Rectangle Cuboid BoundingBox (Abstract)
        class RCBB : public BoundingBox {
        protected:
            glm::vec3 m_size;
        public:
            void compute(std::vector<glm::vec3> vertices) override;

            /**
             * Getter of the minimal value of the RCBB
             * @return min
             */
            glm::vec3 get_min();

            /**
             * Getter of the maximal value of the RCBB
             * @return max
             */
            glm::vec3 get_max();

            AABB *to_AABB() override;
        };
    }
}
#endif //GAME_ENGINE_RCBB_H
