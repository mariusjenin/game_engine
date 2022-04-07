//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_BOUNDINGBOX_H
#define GAME_ENGINE_BOUNDINGBOX_H


#include <glm/vec3.hpp>
#include <vector>

class BoundingBox {
protected:
    glm::vec3 m_position;
public:
    virtual void compute(std::vector<glm::vec3> vertices) = 0;
};


#endif //GAME_ENGINE_BOUNDINGBOX_H
