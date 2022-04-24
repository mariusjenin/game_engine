//
// Created by mariusjenin on 07/04/2022.
//

#ifndef GAME_ENGINE_RIGIDBODY_H
#define GAME_ENGINE_RIGIDBODY_H

class RigidBody {
public:
    virtual ~RigidBody() = 0;
    virtual void update(float delta_time) = 0;
    virtual void apply_forces() = 0;
};


#endif //GAME_ENGINE_RIGIDBODY_H
