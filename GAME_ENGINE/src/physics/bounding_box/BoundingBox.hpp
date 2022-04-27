//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_BOUNDINGBOX_H
#define GAME_ENGINE_BOUNDINGBOX_H


#include <glm/glm.hpp>
#include <vector>

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
            SphereBB_TYPE,
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
             */
            void compute(std::vector<BoundingBox *> bbs);

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
            virtual AABB *to_AABB() = 0;

            /**
             * Compute the Collision with a SphereBB
             * @return collision
             */
            virtual Collision get_data_collision(const SphereBB &bb) = 0;

            /**
             * Compute the Collision with an AABB
             * @return collision
             */
            virtual Collision get_data_collision(const AABB &bb) = 0;

            /**
             * Compute the Collision with an OBB
             * @return collision
             */
            virtual Collision get_data_collision(const OBB &bb) = 0;

            /**
             * Apply the Matrix to the BoundingBox
             * @param matrix
             */
            virtual void apply_transform(glm::mat4 matrix) = 0;
        };
    }
}

#endif //GAME_ENGINE_BOUNDINGBOX_H
