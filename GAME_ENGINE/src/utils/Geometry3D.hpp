
#ifndef GAME_ENGINE_GEOMETRY3D_H
#define GAME_ENGINE_GEOMETRY3D_H


#include <glm/glm.hpp>

namespace physics {
    namespace bounding_box {
        class AABB;

        class OBB;

        class RCBB;
    }
}
using namespace physics::bounding_box;

/**
 * Compare 2 floats
 */
#define cmp_float(x, y)               \
    (fabsf((x)-(y)) <= FLT_EPSILON * \
        fmaxf(1.0f,                  \
        fmaxf(fabsf(x), fabsf(y)))   \
)

namespace utils {

    struct Plane;

    ///Line with a start point and an end point
    struct Line {
        glm::vec3 start;
        glm::vec3 end;

        /**
         * Costructor of a Line with the start point and the end point
         * @param s
         * @param e
         */
        Line(glm::vec3 s, glm::vec3 e);

        /**
         * Getter of the length of the Line
         * @return length
         */
        float length() const;

        /**
         * Get the intersection point of the Line on a Plane if the Line intersects the Plane
         * @param plane
         * @param intersection
         * @return
         */
        bool intersection_plane(Plane plane, glm::vec3 *intersection = nullptr) const;
    };

    ///Ray with an origine and a direction
    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;

        /**
         * Constructor of a Ray with the origin and the direction
         * @param o
         * @param d
         */
        Ray(glm::vec3 o, glm::vec3 d);
    };

    ///Plane with a normal and a distance from the origin along the axis of the normal
    struct Plane {
        glm::vec3 normal;
        ///Distance from origin
        float distance;

        /**
         * Constructor of a Plane with the normal and the distance from the origin
         * @param n
         * @param d
         */
        Plane(glm::vec3 n, float d);

        /**
         * Constructor of a Plane with a normal and a point on the Plane
         * @param n
         * @param p
         */
        static Plane plane_from_normal_point(glm::vec3 n, glm::vec3 p);


        /**
         * Constructor of a Plane with a point on the Plane and 2 vectors non-collinear on the plane
         * @param p
         * @param v1
         * @param v2
         */
        static Plane plane_from_normal_vectors(glm::vec3 p, glm::vec3 v1, glm::vec3 v2);

        /**
         * Constructor of a Plane with a 3 points on the Plane
         * @param v1
         * @param v2
         * @param v3
         * @return
         */
        static Plane plane_from_points(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);


        /**
         * Inform if a point is on the Plane (=0), behind the Plane (<0) or in front of the Plane (>0)
         * @param point
         * @return result of the equation
         */
        float equation(glm::vec3 point) const;
    };

    struct Interval {
        float min;
        float max;

        /**
         * Constructor of an Interval with a min value and a max value
         * @param mi
         * @param ma
         */
        Interval(float mi, float ma);
    };
}
#endif //GAME_ENGINE_GEOMETRY3D_H
