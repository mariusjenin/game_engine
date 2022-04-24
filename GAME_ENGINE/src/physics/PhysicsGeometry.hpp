//
// Created by mariusjenin on 24/04/2022.
//

#ifndef GAME_ENGINE_PHYSICSGEOMETRY_H
#define GAME_ENGINE_PHYSICSGEOMETRY_H

#include "src/physics/bounding_box/RCBB.hpp"
#include "src/physics/bounding_box/SphereBB.hpp"
#include <glm/glm.hpp>

#define cmp_float(x,y)               \
    (fabsf((x)-(y)) <= FLT_EPSILON * \
        fmaxf(1.0f,                  \
        fmaxf(fabsf(x), fabsf(y)))   \
)

struct Collision{
    bool colliding;
    glm::vec3 normal;
    float depth;
    std::vector<glm::vec3> contacts;

    Collision(){
        colliding = false;
        normal = {0,0,1};
        depth = FLT_MAX;
        contacts = {};
    }

    void reset(){
        colliding = false;
        normal = {0,0,1};
        depth = FLT_MAX;
        contacts.clear();
    }
};


//Collision get_data_collision(RCBB, RCBB);
Collision get_data_collision(const SphereBB&, const SphereBB&);

#endif //GAME_ENGINE_PHYSICSGEOMETRY_H
