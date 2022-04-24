//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_BOUNDINGBOX_H
#define GAME_ENGINE_BOUNDINGBOX_H


#include <glm/vec3.hpp>
#include <vector>
namespace physics {
    /**
     * Enum of the different types of BoundingBox
     */
    enum BB_TYPE {
        AABB_TYPE,
        OBB_TYPE,
        SphereBB_TYPE,
    };

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
    };
}

#endif //GAME_ENGINE_BOUNDINGBOX_H
