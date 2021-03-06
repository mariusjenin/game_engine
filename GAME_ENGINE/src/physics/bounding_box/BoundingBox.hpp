//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_BOUNDINGBOX_H
#define GAME_ENGINE_BOUNDINGBOX_H


#include <glm/glm.hpp>
#include <vector>
#include <src/utils/Geometry3D.hpp>
#include <src/utils/Transform.hpp>

namespace utils {
    struct Interval;
}
using namespace utils;

namespace physics {
    struct Collision;

    namespace bounding_box {
        struct AABB;
        struct SphereBB;
        struct OBB;
        /**
         * Enum of the different types of BoundingBox
         */
        enum BB_TYPE {
            AABB_TYPE,
            OBB_TYPE,
            SPHEREBB_TYPE,
        };
        ///Represent a bounding box (Abstract)
        class BoundingBox {
        protected:
            glm::vec3 m_position;
            BB_TYPE m_type;
        public:

            /**
             * Compute the BoundingBox according to vertices
             * @param vertices
             */
            virtual void compute(std::vector<glm::vec3> vertices) = 0;

            /**
             * Compute the BoundingBox according to a list of BoundingBox
             * @param bbs
             * @return has computed
             */
            bool compute(std::vector<BoundingBox *> bbs);

            /**
             * Getter of the type of the BoundingBox
             * @return type
             */
            BB_TYPE get_type() const;

            /**
             * Getter of the position of the BoundingBox
             * @return position
             */
            const glm::vec3 &get_position() const;

            /**
             * Setter of the position of the BoundingBox
             * @param position
             */
            void set_position(const glm::vec3 &position);

            /**
             * Convert the BoundingBox to an AABB
             * @return aabb
             */
            virtual AABB *to_AABB() const = 0;

            /**
             * Compute the Collision with a SphereBB
             * @param bb
             * @return collision
             */
            virtual Collision get_data_collision(SphereBB *bb) = 0;

            /**
             * Compute the Collision with an RCBB
             * @param bb
             * @return collision
             */
            virtual Collision get_data_collision(RCBB *bb) = 0;

            /**
             * Apply the Matrix to the BoundingBox
             * @param matrix
             */
            virtual void apply_transform(glm::mat4 matrix) = 0;

            /**
             * Get the Interval clip on an axis
             * @param axis
             * @return interval
             */
            virtual Interval get_interval(glm::vec3 axis) = 0;

            /**
             * Give the vertices that compose the BoundingBox
             * @return
             */
            virtual std::vector<glm::vec3> to_vertices() const = 0;

            /**
             * Returns intersection distance along ray direction between ray and boundingbox.
             * @return
             */
            virtual float is_intersected(Ray) = 0;

            /**
             * Returns return closest point on bounding box to pt.
             * @return
             */
            virtual glm::vec3 closest_point(glm::vec3 pt) const = 0;
            
            /**
             * Getter of the tensor matrix
             * @return tensor matrix
             */
            virtual glm::vec3 get_tensor() = 0;
            
            /**
             * Getter of the max distance between two points in a bb
             * @return float distance
             */
            virtual float get_max_dist() = 0;


        };
    }
}

#endif //GAME_ENGINE_BOUNDINGBOX_H
