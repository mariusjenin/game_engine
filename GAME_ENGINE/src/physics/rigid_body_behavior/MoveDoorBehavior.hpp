#ifndef GAME_ENGINE_MOVEDOORBEHAVIOR_H
#define GAME_ENGINE_MOVEDOORBEHAVIOR_H

#include "RigidBodyBehavior.hpp"
#include <src/physics/PhysicsSystem.hpp>
#include <src/physics/force/Force.hpp>
#include "src/game_element/DoorElement.hpp"

namespace physics {
    namespace rigid_body_behavior {
        class MoveDoorBehavior : public RigidBodyBehavior{
        private:
            DoorElement* m_door;
            float m_timer;
            std::vector<RigidBodyVolume*> m_rigid_bodies;
        public:
            /**
             * Constructor of MoveDoorBehavior
             * @param door
             */
            explicit MoveDoorBehavior(DoorElement* door);
            void action(PhysicsSystem* ps,Collision collision,float delta_time) override;
            void update_physics(float delta_time) override;
            void update_render(float delta_time, ODE* ode) override;

            /**
             * Specify a collider that trigger action of the MoveDoorBehavior
             * @param rbv
             */
            void can_collide_with(RigidBodyVolume* rbv);
        };
    }
}


#endif 
