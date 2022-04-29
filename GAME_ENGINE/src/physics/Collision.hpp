
#ifndef GAME_ENGINE_COLLISION_H
#define GAME_ENGINE_COLLISION_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "src/utils/Geometry3D.hpp"

namespace physics {
    class RigidBodyVolume;

    /**
     * Represents a collision between 2 \link bounding_box::BoundingBox BoundingBox\endlink
     */
    struct Collision {
        bool colliding;
        glm::vec3 normal;
        float depth;
        std::vector<glm::vec3> contacts;
        RigidBodyVolume* rigid_body_1{};
        RigidBodyVolume* rigid_body_2{};

        /**
         * Constructor of a Collision
         */
        Collision() {
            colliding = false;
            normal = {0, 0, 1};
            depth = FLT_MAX;
            contacts = {};
        }

        /**
         * Resets the attributes of a Collision
         */
        void reset() {
            colliding = false;
            normal = {0, 0, 1};
            depth = FLT_MAX;
            contacts.clear();
        }

        /**
         * Give the string that corresponds to the Collision
         * @return collision as string
         */
        std::string  to_string();

    };
}



#endif //GAME_ENGINE_COLLISION_H
