//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_RCBB_H
#define GAME_ENGINE_RCBB_H

#include "BoundingBox.hpp"
#include <cfloat>

class RCBB : public BoundingBox {
private:
    glm::vec3 m_size;
public:
    void compute(std::vector<glm::vec3> vertices) override;
    glm::vec3 get_min();
    glm::vec3 get_max();
};


#endif //GAME_ENGINE_RCBB_H
