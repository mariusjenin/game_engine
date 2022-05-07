//
// Created by mariusjenin on 07/05/2022.
//

#ifndef GAME_ENGINE_RIGIDBODYBEHAVIOR_H
#define GAME_ENGINE_RIGIDBODYBEHAVIOR_H

#include <src/physics/RigidBodyVolume.hpp>

namespace physics {
    class PhysicsSystem;
    namespace ode{
        class ODE;
    }
    using namespace ode;
    namespace rigid_body_behavior {

        enum RigidBodyBehavior_TYPE{
            MovementBehavior_TYPE,
            SwitchColor_TYPE
        };

        class RigidBodyBehavior {
        protected:
            RigidBodyVolume* m_rigid_body{};
            RigidBodyBehavior_TYPE m_type{};
        public:
            /**
             * Function called each time that the RigidBodyVolume collide
             * @param ps
             * @param collision
             * @param delta_time
             */
            virtual void action(PhysicsSystem* ps,Collision collision,float delta_time) = 0;

            /**
             * Function called each time the physic is updated
             * @param delta_time
             */
            virtual void update_physics(float delta_time) = 0;

            /**
             * Function called each time the rendering is updated
             * @param delta_time
             * @param ode
             */
            virtual void update_render(float delta_time, ODE* ode) = 0;

            /**
             * Setter of the RigidBodyVolume
             * @param rbv
             */
            void set_rigid_body(RigidBodyVolume* rbv);

            /**
             * Getter of the type
             * @return type
             */
            RigidBodyBehavior_TYPE get_type() const;
        };
    }
}


#endif //GAME_ENGINE_RIGIDBODYBEHAVIOR_H
