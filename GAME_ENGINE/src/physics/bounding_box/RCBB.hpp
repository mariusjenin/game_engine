//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_RCBB_H
#define GAME_ENGINE_RCBB_H

#include "BoundingBox.hpp"
#include <cfloat>
// #include <src/utils/Geometry3D.hpp>

namespace utils {
    struct Line;
    struct Plane;
}
namespace physics {

    namespace bounding_box {
        class BoundingBox;
        class AABB;

        /// Rectangle Cuboid BoundingBox (Abstract)
        class RCBB : public BoundingBox {
        protected:
            glm::vec3 m_size;
        public:
            void compute(std::vector<glm::vec3> vertices) override;

            /**
             * Give the edges that compose the RCBB
             * @return
             */
            std::vector<Line> to_edges() const;

            /**
             * Give the planes that compose the RCBB
             * @return
             */
            std::vector<Plane> to_planes() const;

            /**
             * Get all the intersections with a list of Line
             * @param edges
             * @return intersections
             */
            std::vector<glm::vec3> get_intersections_lines(std::vector<Line> lines) const;

            /**
             * Check if a point is in the RCBB
             * @param point
             * @return is_point_in
             */
            virtual bool is_point_in(glm::vec3 point) const = 0;


            /**
             * Compute the depth of the penetration between 2 RCBB colliding
             * @param bb
             * @param axis
             * @param out_should_flip
             * @return depth
             */
            float penetrate_depth(RCBB* bb,glm::vec3 axis, bool* out_should_flip);

            AABB *to_AABB() const override;

            Interval get_interval(glm::vec3 axis) override;

            virtual float is_intersected(Ray) = 0;

            glm::vec3 get_tensor() override;
        };
    }
}
#endif //GAME_ENGINE_RCBB_H
