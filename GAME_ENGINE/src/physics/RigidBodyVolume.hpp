#ifndef GAME_ENGINE_RIGIDBODYVOLUME_H
#define GAME_ENGINE_RIGIDBODYVOLUME_H

#include "RigidBody.hpp"
#include "bounding_box/BoundingBox.hpp"
#include "bounding_box/SphereBB.hpp"
#include "src/scene_graph/NodeGameSG.hpp"

namespace scene_graph {
    class NodeGameSG;
}

using namespace scene_graph;

namespace physics {
    struct Collision;
    class RigidBodyVolume : public RigidBody {
    private:
        NodeGameSG* m_node_game;
        glm::vec3 m_velocity;
        glm::vec3 m_forces;
        float m_mass{};
        /// Coefficient of restitution
        float m_cor{};
        float m_friction{};
    public:
        /**
         * Constructor of a RigidBodyVolume with a NodeGameSG and configurable coefficients
         * @param ng
         * @param mass
         * @param friction
         * @param cor
         */
        RigidBodyVolume(NodeGameSG *ng , float mass = 1.0f, float friction = 0.6f, float cor = 0.5f);

        /**
         * Destructor of a RigidBodyVolume
         */
        ~RigidBodyVolume() override;

        void update(float delta_time) override;

        void apply_forces() override;

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
        void apply_impulse(RigidBodyVolume &rbv, const Collision& collision);

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
        NodeGameSG* get_node();
    };

}
#endif //GAME_ENGINE_RIGIDBODYVOLUME_H
