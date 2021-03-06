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
            std::vector<RigidBodyVolume*> m_rigid_bodies;
            float m_timer;
            void switch_color();
        public:
            /**
             * Constructor of SwitchColorBehavior
             * @param material
             */
            explicit SwitchColorBehavior(Material* material);
            void action(PhysicsSystem* ps,Collision collision,float delta_time) override;
            void update_physics(float delta_time) override;
            void update_render(float delta_time, ODE* ode) override;
            /**
             * Specify a collider that trigger action of the SwitchColorBehavior
             * @param rbv
             */
            void can_collide_with(RigidBodyVolume* rbv);
        };
    }
}


#endif //GAME_ENGINE_SWITCHCOLORBEHAVIOR_H
