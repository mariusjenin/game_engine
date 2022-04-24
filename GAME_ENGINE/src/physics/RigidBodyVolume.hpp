#ifndef GAME_ENGINE_RIGIDBODYVOLUME_H
#define GAME_ENGINE_RIGIDBODYVOLUME_H


#include "RigidBody.hpp"
#include "PhysicsGeometry.hpp"

class RigidBodyVolume : public RigidBody {
private:
    BoundingBox *m_bb{};
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_forces;
    float m_mass{};
    /// Coefficient of restitution
    float m_cor{};
    float m_friction{};

public:
    RigidBodyVolume();

    ~RigidBodyVolume() override;

    void update(float delta_time) override;
    void apply_forces() override;

    void synchro_collision_volumes();
    float inverse_mass() const;
    void add_linear_impulse(glm::vec3& impulse);
    void apply_impulse(RigidBodyVolume& rbv, Collision c);

    Collision find_data_collision(RigidBodyVolume& rbv);

    BoundingBox *get_bb() const;
};


#endif //GAME_ENGINE_RIGIDBODYVOLUME_H
