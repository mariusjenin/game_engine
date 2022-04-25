
#ifndef GAME_ENGINE_RIGIDBODY_H
#define GAME_ENGINE_RIGIDBODY_H
namespace physics {
    class RigidBody {
    public:
        /**
         * Destructor of a RigidBody
         */
        virtual ~RigidBody() = 0;

        /**
         * Update the RigidBody according to the delta time
         * @param delta_time
         */
        virtual void update(float delta_time) = 0;

        /**
         * Apply the forces on the RigidBody
         */
        virtual void apply_forces() = 0;
    };
}

#endif //GAME_ENGINE_RIGIDBODY_H
