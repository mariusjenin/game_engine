//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_SPHEREBB_H
#define GAME_ENGINE_SPHEREBB_H


#include "BoundingBox.hpp"
#include <cfloat>

class SphereBB : public BoundingBox {
private:
    float m_radius;
public:
    void compute(std::vector<glm::vec3> vertices) override;

    float get_radius() const;
};


#endif //GAME_ENGINE_SPHEREBB_H
