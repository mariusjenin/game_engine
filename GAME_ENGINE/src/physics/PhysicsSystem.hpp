
#ifndef GAME_ENGINE_PHYSICSSYSTEM_H
#define GAME_ENGINE_PHYSICSSYSTEM_H

#include <vector>
#include "Collision.hpp"
#include "src/physics/ode/ODE.hpp"
#include "src/scene_graph/NodeGameSG.hpp"
namespace scene_graph{
    class ElementSG;
    class NodeGameSG;
}
using namespace scene_graph;
namespace physics{

    using namespace ode;
    /// Manage a whole physic system with RigidBody
    class PhysicsSystem {
    private:
        std::vector<RigidBodyVolume *> m_colliders;
        ElementSG* m_root_physics;
        float m_linear_projection_percent;
        float m_penetration_slack;
        int m_impulse_iteration;
        float m_multiplicator;
        ODE* m_ode;
    public:
        /**
         * Constructor of PhysicsSystem
         * @param lpp
         * @param ps
         * @param ii
         */
        explicit PhysicsSystem(ElementSG* root_physics,float mult_physics = 1.0f, float lpp = 0.2f, float ps=0.01f, int ii=5, ODE_TYPE ode_type = EULER_TYPE);

        /**
         * Add a RigidBodyVolume to the PhysicsSystem
         * @param rbv
         */
        void add_collider(RigidBodyVolume* rbv);

        /**
         * Remove a RigidBodyVolume to the PhysicsSystem
         * @param rbv
         */
        void remove_collider(RigidBodyVolume* rbv);

        /**
         * Clear all the RigidBodyVolume
         */
        void clear_rigid_bodies();

        /**
         * Clear the RigidBodyVolume with the NodeGameSG given
         * @param node
         */
        void remove_collider_with_node(NodeGameSG* node);

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

        /**
         * Getter of the impulse iteration
         * @return impulse iteration
         */
        int get_impulse_iteration() const;

        /**
         * Getter of the penetration slack
         * @return penetration slack
         */
        float get_penetration_slack() const;

        /**
         * Getter of the linear projection percent
         * @return linear projection percent
         */
        float get_linear_projection_percent() const;

        /**
         * Setter of the Physics forces multiplicator
         * @param mult_physics
         */
        void set_multiplicator_physics(float mult_physics);

        /**
         * Getter of the Physics forces multiplicator
         * @return multiplicator
         */
        float get_multiplicator_physics() const;
    };
}


#endif //GAME_ENGINE_PHYSICSSYSTEM_H
