//
// Created by mariusjenin on 07/05/2022.
//

#ifndef GAME_ENGINE_MOVEMENTBEHAVIOR_H
#define GAME_ENGINE_MOVEMENTBEHAVIOR_H

#include "RigidBodyBehavior.hpp"
#include <src/physics/PhysicsSystem.hpp>
#include <src/physics/force/Force.hpp>

namespace physics {
    namespace force{
        class Force;
    }
    using namespace force;
    namespace rigid_body_behavior {
        class MovementBehavior : public RigidBodyBehavior{
        private:
            glm::vec3 m_velocity;
            glm::vec3 m_angular_velocity;
            glm::vec3 m_acceleration;
            glm::vec3 m_angular_acceleration;
            glm::vec3 m_forces;
            glm::vec3 m_torques;
            float m_mass{};
            /// Coefficient of restitution
            float m_cor{};
            float m_friction{};
            std::vector<Force*> m_list_forces;
        public:

            explicit MovementBehavior(float mass = 1.0f, float friction = 0.6f, float cor = 0.5f);


            void action(PhysicsSystem* ps,Collision collision,float delta_time) override;
            void update_physics(float delta_time) override;
            void update_render(float delta_time, ODE* ode) override;

            /**
             * Add a force to the list of force to apply at each updates
             * @param f
             */
            void add_force(Force* f);

            /**
             * Getter of the inverse of the mass
             * @return inverse mass
             */
            float inverse_mass() const;

            /**
             * Getter of the inverse of the tensor
             * @return inverse tensor
             */
            glm::mat4 inverse_tensor() const;

            /**
             * Add a linear impulse to the RigidBodyVolume
             * @param impulse
             */
            void add_linear_impulse(glm::vec3 &impulse);

            /**
             * set a linear impulse to the RigidBodyVolume
             * @param impulse
             */
            void set_linear_impulse(glm::vec3 &impulse);

            /**
             * Add a rotational impulse to the RigidBodyVolume
             * @param point
             * @param impulse
             */
            void add_rotational_impulse(glm::vec3 &point, glm::vec3 &impulse);

            /**
             * Apply an impulse to the RigidBodyVolume
             * @param rbv
             * @param collision
             */
            void apply_impulse(RigidBodyVolume &rbv, const Collision &collision, int index_contact);

            /**
             * Getter of the mass
             * @return mass
             */
            float get_mass() const;

            /**
             * Setter of the mass
             * @return mass
             */
            void set_mass(float);

            /**
             * Setter of the forces
             * @param forces
             */
            void set_forces(const glm::vec3 &forces);

            /**
             * Clear the list of forces
             */
            void clear_forces();

            /**
             * Getter of the forces
             * @return forces
             */
            glm::vec3 get_forces() const;

            /**
             * Getter of the torques
             * @return torques
             */
            glm::vec3 get_torques() const;

            /**
             * Getter of the velocity
             * @return velocity
             */
            glm::vec3 get_velocity() const;


            /**
             * Getter of the velocity
             * @return velocity
             */
            glm::vec3 get_acceleration() const;


            /**
             * Setter of the velocity
             * @param velocity
             */
            void set_velocity(const glm::vec3 &velocity);

            /**
             * Setter of the angular velocity
             * @param angular_velocity
             */
            void set_angular_velocity(const glm::vec3 &angular_velocity);

            /**
             * Setter of the acceleration
             * @param acceleration
             */
            void set_acceleration(const glm::vec3 &acceleration);

            /**
             * Getter of the angular velocity
             * @return angular velocity
             */
            glm::vec3 get_angular_velocity() const;

            /**
             * Getter of the angular acceleration
             * @return angular acceleration
             */
            glm::vec3 get_angular_acceleration() const;
        };
    }
}


#endif //GAME_ENGINE_MOVEMENTBEHAVIOR_H
