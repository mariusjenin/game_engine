#ifndef GAME_ENGINE_RIGIDBODYVOLUME_H
#define GAME_ENGINE_RIGIDBODYVOLUME_H

#include "src/physics/bounding_box/BoundingBox.hpp"
#include "src/physics/bounding_box/SphereBB.hpp"
#include "src/physics/bounding_box/OBB.hpp"
#include "src/scene_graph/NodeGameSG.hpp"
#include "src/physics/force/Force.hpp"

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

    /// Represents a rigid body with a volume (\link bounding_box::BoundingBox BoundingBox\endlink)
    class RigidBodyVolume {
    private:
        NodeGameSG *m_node_game;
        glm::vec3 m_velocity;
        glm::vec3 m_acceleration;
        glm::vec3 m_forces;
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
         */
        void update(float delta_time);

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
         * Add a linear impulse to the RigidBodyVolume
         * @param impulse
         */
        void add_linear_impulse(glm::vec3 &impulse);

        /**
         * Apply an impulse to the RigidBodyVolume
         * @param rbv
         * @param collision
         */
        void apply_impulse(RigidBodyVolume &rbv, const Collision &collision);

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
         * @return
         */
        glm::vec3 get_forces() const;
    };
}
#endif //GAME_ENGINE_RIGIDBODYVOLUME_H
