//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_SPHEREBB_H
#define GAME_ENGINE_SPHEREBB_H


#include "BoundingBox.hpp"
#include <cfloat>
#include <src/physics/Collision.hpp>

namespace physics {

    namespace bounding_box {

        /// Sphere BoundingBox
        class SphereBB : public BoundingBox {
        private:
            float m_radius{};
        public:
            /**
             * Constructor of SphereBB
             */
            SphereBB();

            void compute(std::vector<glm::vec3> vertices) override;

            /**
             * Getter of the radius of the SphereBB
             * @return
             */
            float get_radius() const;

            void apply_transform(glm::mat4 matrix) override;

            Collision get_data_collision(SphereBB *bb) override;

            Collision get_data_collision(RCBB *bb) override;

            AABB * to_AABB() const override;

            Interval get_interval(glm::vec3 axis) override;

            std::vector<glm::vec3> to_vertices() const override;

            float is_intersected(Ray) override;

            glm::vec3 closest_point(glm::vec3 pt) const override;

            glm::vec3 get_tensor() override;

            float get_max_dist() override;
        };
    }
}

#endif //GAME_ENGINE_SPHEREBB_H
