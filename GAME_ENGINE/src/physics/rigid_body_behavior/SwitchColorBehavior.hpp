//
// Created by mariusjenin on 07/05/2022.
//

#ifndef GAME_ENGINE_SWITCHCOLORBEHAVIOR_H
#define GAME_ENGINE_SWITCHCOLORBEHAVIOR_H

#include "RigidBodyBehavior.hpp"
#include <src/physics/PhysicsSystem.hpp>
#include <src/physics/force/Force.hpp>

namespace physics {
    namespace rigid_body_behavior {
        class SwitchColorBehavior : public RigidBodyBehavior{
        private:
            Material* m_material;
            float m_timer;
            void switch_color();
        public:
            explicit SwitchColorBehavior(Material* material);
            void action(PhysicsSystem* ps,Collision collision,float delta_time) override;
            void update_physics(float delta_time) override;
            void update_render(float delta_time, ODE* ode) override;
        };
    }
}


#endif //GAME_ENGINE_SWITCHCOLORBEHAVIOR_H
