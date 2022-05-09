#include "RigidBodyVolume.hpp"
#include "src/physics/Collision.hpp"
#include "src/physics/ode/ODEFactory.hpp"
#include "src/utils/printer.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include "src/physics/rigid_body_behavior/MovementBehavior.hpp"

using namespace physics;
using namespace physics::force;
using namespace physics::rigid_body_behavior;
using namespace scene_graph;

RigidBodyVolume::RigidBodyVolume(NodeGameSG *ng, bool character) {
    m_node_game = ng;
    m_node_game->set_rigid_body(this);
}

RigidBodyVolume::~RigidBodyVolume() = default;

Collision RigidBodyVolume::find_data_collision(RigidBodyVolume &rbv) {
    Collision collision;

    BoundingBox *bb1 = m_node_game->get_bb();
    BoundingBox *bb2 = rbv.m_node_game->get_bb();
    switch (bb1->get_type()) {
        case SPHEREBB_TYPE: {
            switch (bb2->get_type()) {
                case SPHEREBB_TYPE:
                    collision = ((SphereBB *) bb1)->get_data_collision((SphereBB *) bb2);
                    break;
                case OBB_TYPE:
                case AABB_TYPE:
                    collision = ((SphereBB *) bb1)->get_data_collision((RCBB *) bb2);
//                    collision.normal*=-1;
                    break;
                default:
                    break;
            }
            break;
        }
        case OBB_TYPE:
        case AABB_TYPE: {
            switch (bb2->get_type()) {
                case SPHEREBB_TYPE:
                    collision = ((RCBB *) bb1)->get_data_collision((SphereBB *) bb2);
                    break;
                case OBB_TYPE:
                case AABB_TYPE:
                    collision = ((RCBB *) bb1)->get_data_collision((RCBB *) bb2);
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            collision = Collision();
            break;
    }

    glm::vec3 normal = collision.normal;
    collision.rigid_body_1 = this;
    collision.rigid_body_2 = &rbv;
    return collision;
}

NodeGameSG *RigidBodyVolume::get_node() {
    return m_node_game;
}

bool RigidBodyVolume::has_movement_behavior() {
    return m_movement_behavior != nullptr;
}

MovementBehavior *RigidBodyVolume::get_movement_behavior() {
    return m_movement_behavior;
}

void RigidBodyVolume::add_behavior(RigidBodyBehavior *behavior) {
    if (behavior->get_type() == MovementBehavior_TYPE) {
        int index_to_remove = -1;
        size_t size_behaviors = m_behaviors.size();
        for (int i = 0; i < size_behaviors; i++) {
            if (m_behaviors.at(i)->get_type() == MovementBehavior_TYPE)index_to_remove = i;
        }
        if (index_to_remove >= 0) m_behaviors.erase(m_behaviors.begin() + index_to_remove);
        m_movement_behavior = (MovementBehavior *) behavior;
    }
    behavior->set_rigid_body(this);
    m_behaviors.push_back(behavior);
}

void RigidBodyVolume::update_physics(float delta_time) {
    for (RigidBodyBehavior *behav: m_behaviors) {
        behav->update_physics(delta_time);
    }
}

void RigidBodyVolume::update_render(float delta_time, ODE *ode) {
    for (RigidBodyBehavior *behav: m_behaviors) {
        behav->update_render(delta_time, ode);
    }
}

void RigidBodyVolume::action(PhysicsSystem *ps, const Collision &collision, float delta_time) {
    for (RigidBodyBehavior *behav: m_behaviors) {
        behav->action(ps, collision, delta_time);
    }
}


