//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_SPHEREBB_H
#define GAME_ENGINE_SPHEREBB_H


#include "BoundingBox.hpp"
#include <cfloat>
#include <src/physics/Collision.hpp>

namespace physics {
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

        Collision get_data_collision(const SphereBB &bb) override;
        Collision get_data_collision(const AABB &bb) override;
        Collision get_data_collision(const OBB &bb) override;

        AABB* to_AABB() override;
    };
}

#endif //GAME_ENGINE_SPHEREBB_H
