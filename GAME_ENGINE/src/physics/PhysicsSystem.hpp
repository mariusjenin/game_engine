
#ifndef GAME_ENGINE_PHYSICSSYSTEM_H
#define GAME_ENGINE_PHYSICSSYSTEM_H

#include <vector>
#include "Collision.hpp"
#include "src/physics/ode/ODE.hpp"
#include "src/scene_graph/NodeGameSG.hpp"

namespace physics{

    using namespace ode;
    /// Manage a whole physic system with RigidBody
    class PhysicsSystem {
    private:
        std::vector<RigidBodyVolume *> m_rigid_bodies;
        std::vector<Collision> m_collisions;
        ElementSG* m_root_physics;
        float m_linear_projection_percent;
        float m_penetration_slack;
        int m_impulse_iteration;
        ODE* m_ode;
    public:
        /**
         * Constructor of PhysicsSystem
         * @param lpp
         * @param ps
         * @param ii
         */
        explicit PhysicsSystem(ElementSG* root_physics,float lpp = 0.2f, float ps=0.01f, int ii=5, ODE_TYPE ode_type = EULER_TYPE);

        /**
         * Add a RigidBodyVolume to the PhysicsSystem
         * @param rbv
         */
        void add_rigid_body(RigidBodyVolume* rbv);

        /**
         * Remove a RigidBodyVolume to the PhysicsSystem
         * @param rbv
         */
        void remove_rigid_body(RigidBodyVolume* rbv);

        /**
         * Clear all the RigidBodyVolume
         */
        void clear_rigid_bodies();

        /**
         * Clear the RigidBodyVolume with the NodeGameSG given
         * @param node
         */
        void remove_rigid_body_with_node(NodeGameSG* node);

        /**
         * Update the Collisions in the PhysicsSystem
         * @param pos_camera
         * @param delta_time
         */
        void update_collisions(glm::vec3 pos_camera,float delta_time);

        /**
         * Update the RigidBodyVolume in the PhysicsSystem
         * @param pos_camera
         * @param delta_time
         */
        void update_bodies(glm::vec3 pos_camera,float delta_time);

        /**
         * Refresh the BoundingBox of each RigidBodyVolume
         * @param pos_camera
         */
        void refresh_bodies_bb(glm::vec3 pos_camera);
    };
}


#endif //GAME_ENGINE_PHYSICSSYSTEM_H
