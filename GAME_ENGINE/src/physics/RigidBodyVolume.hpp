#ifndef GAME_ENGINE_RIGIDBODYVOLUME_H
#define GAME_ENGINE_RIGIDBODYVOLUME_H


#include "RigidBody.hpp"
#include "PhysicsGeometry.hpp"

namespace physics {
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
        /**
         * Constructor of a RigidBodyVolume
         */
        RigidBodyVolume();

        /**
         * Destructor of a RigidBodyVolume
         */
        ~RigidBodyVolume() override;

        void update(float delta_time) override;

        void apply_forces() override;

        /**
         * Synchronize the Bounding to be like the RigidBodyVolume
         */
        void synchro_collision_volumes();

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
         * @param c
         */
        void apply_impulse(RigidBodyVolume &rbv, Collision c);

        /**
         * Compute if there is a Collision between this RigidBodyVolume and another
         * @param rbv
         * @return collision
         */
        Collision find_data_collision(RigidBodyVolume &rbv);

        /**
         * Getter of the %BoundingBox
         * @return bb
         */
        BoundingBox *get_bb() const;
    };

}
#endif //GAME_ENGINE_RIGIDBODYVOLUME_H
