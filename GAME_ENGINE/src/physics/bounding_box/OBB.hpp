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
        private:
            glm::mat3 m_orientation;
        public:
            /**
             * Constructor of an OBB
             */
            OBB();

            void compute(std::vector<glm::vec3> vertices) override;

            glm::mat3 get_orientation() const override;

            std::vector<glm::vec3> to_vertices() const override;

            void apply_transform(glm::mat4 matrix) override;

            float get_max_dist() override;

            
        };
    }
}
#endif //GAME_ENGINE_OBB_H
