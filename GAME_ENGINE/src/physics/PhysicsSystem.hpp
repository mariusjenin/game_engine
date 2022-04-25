
#ifndef GAME_ENGINE_PHYSICSSYSTEM_H
#define GAME_ENGINE_PHYSICSSYSTEM_H

#include <vector>
#include "Collision.hpp"


namespace physics{

    class PhysicsSystem {
    private:
        std::vector<RigidBodyVolume *> m_rigid_bodies;
        std::vector<Collision> m_collisions;
        float m_linear_projection_percent;
        float m_penetration_slack;
        int m_impulse_iteration;
    public:
        explicit PhysicsSystem(float lpp = 0.45f, float ps=0.01f, int ii=5);
        void add_rigid_body(RigidBodyVolume* rbv);
        void remove_rigid_body(RigidBodyVolume* rbv);
        void update(glm::vec3 pos_camera,float delta_time);
    };
}


#endif //GAME_ENGINE_PHYSICSSYSTEM_H
