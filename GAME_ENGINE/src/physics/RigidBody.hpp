//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_RIGIDBODY_H
#define GAME_ENGINE_RIGIDBODY_H

#include "vector"
#include "BoundingBox.hpp"

class RigidBody {
public:
//    RigidBody();
    virtual ~RigidBody() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render() = 0;
    virtual void apply_forces() = 0;
    virtual void solve_constraints(std::vector<BoundingBox> &bbs) = 0;
};


#endif //GAME_ENGINE_RIGIDBODY_H
