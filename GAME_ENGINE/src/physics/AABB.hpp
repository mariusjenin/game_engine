//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_AABB_H
#define GAME_ENGINE_AABB_H


#include "BoundingBox.hpp"

class AABB : public BoundingBox {
private:
    glm::vec3 m_size;
public:
    void compute(std::vector<glm::vec3> vertices) override;
    glm::vec3 get_min();
    glm::vec3 get_max();
};


#endif //GAME_ENGINE_AABB_H
