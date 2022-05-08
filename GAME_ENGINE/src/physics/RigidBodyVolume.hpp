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
using namespace scene_graph;

namespace physics {
    class PhysicsSystem;
    namespace force {
        class Force;
    }
    namespace rigid_body_behavior {
        class MovementBehavior;
        class RigidBodyBehavior;
    }
    using namespace force;
    using namespace rigid_body_behavior;
    struct Collision;
    using namespace ode;

    /// Represents a rigid body with a volume (\link bounding_box::BoundingBox BoundingBox\endlink)
    class RigidBodyVolume {
    private:
        NodeGameSG *m_node_game;

        std::vector<RigidBodyBehavior*> m_behaviors{};
        MovementBehavior* m_movement_behavior{};

    public:
        /**
         * Constructor of a RigidBodyVolume with a NodeGameSG and configurable coefficients
         * @param ng
         * @param mass
         * @param friction
         * @param cor
         */
        RigidBodyVolume(NodeGameSG *ng, bool character=false);

        /**
         * Destructor of a RigidBodyVolume
         */
        ~RigidBodyVolume();
       
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
         * Add a RigidBodyBehavior to the RigidBodyVolume
         * @param behavior
         */
        void add_behavior(RigidBodyBehavior* behavior);

        /**
         * Getter of whether or not the node has a MovementBehavior
         * @return has movement behavior
         */
        bool has_movement_behavior();

        /**
         * Getter of the MovementBehavior
         * @return movement behavior
         */
        MovementBehavior* get_movement_behavior();

        /**
         * Call the action function of each RigidBodyBehavior of the RigidBodyVolume
         * @param ps
         * @param delta_time
         */
        void action(PhysicsSystem* ps,const Collision& collision,float delta_time);

        /**
         * Call the update_physics function of each RigidBodyBehavior of the RigidBodyVolume
         * @param delta_time
         */
        void update_physics(float delta_time);

        /**
         * Call the update_render function of each RigidBodyBehavior of the RigidBodyVolume
         * @param delta_time
         * @param ode
         */
        void update_render(float delta_time, ODE* ode);
    };
}
#endif //GAME_ENGINE_RIGIDBODYVOLUME_H
