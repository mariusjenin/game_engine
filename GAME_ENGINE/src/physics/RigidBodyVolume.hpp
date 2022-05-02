#ifndef GAME_ENGINE_RIGIDBODYVOLUME_H
#define GAME_ENGINE_RIGIDBODYVOLUME_H

#include "src/physics/bounding_box/BoundingBox.hpp"
#include "src/physics/bounding_box/SphereBB.hpp"
#include "src/physics/bounding_box/OBB.hpp"
#include "src/scene_graph/NodeGameSG.hpp"
#include "src/physics/force/Force.hpp"
#include "src/physics/ode/ODE.hpp"


namespace scene_graph {
    class NodeGameSG;
}
namespace force {
}

using namespace scene_graph;

namespace physics {
    namespace force {
        class Force;
    }
    using namespace force;
    struct Collision;
    using namespace ode;

    /// Represents a rigid body with a volume (\link bounding_box::BoundingBox BoundingBox\endlink)
    class RigidBodyVolume {
    private:
        NodeGameSG *m_node_game;
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
        /**
         * Constructor of a RigidBodyVolume with a NodeGameSG and configurable coefficients
         * @param ng
         * @param mass
         * @param friction
         * @param cor
         */
        RigidBodyVolume(NodeGameSG *ng, float mass = 1.0f, float friction = 0.6f, float cor = 0.5f);

        /**
         * Destructor of a RigidBodyVolume
         */
        ~RigidBodyVolume();

        /**
         * Add a force to the list of force to apply at each updates
         * @param f
         */
        void add_force(Force* f);

        /**
         * Update the RigidBodyVolume according to the delta time
         * @param delta_time
         * @param ode
         */
        void update(float delta_time,ODE* ode);

        /**
         * Apply the force on the RigidBodyVolume
         */
        void apply_forces();

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
         * Compute if there is a Collision between this RigidBodyVolume and another
         * @param rbv
         * @return collision
         */
        Collision find_data_collision(RigidBodyVolume &rbv);

        /**
         * Getter of the NodeGameSG
         * @return node
         */
        NodeGameSG *get_node();

        /**
         * Getter of the mass
         * @return mass
         */
        float get_mass() const;

        /**
         * Setter of the forces
         * @param forces
         */
        void set_forces(const glm::vec3 &forces);

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
#endif //GAME_ENGINE_RIGIDBODYVOLUME_H
