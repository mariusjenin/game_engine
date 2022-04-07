//
// Created by marius.jenin@etu.umontpellier.fr on 17/02/2022.
//

#ifndef GAME_ENGINE_TRANSFORM_HPP
#define GAME_ENGINE_TRANSFORM_HPP

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#define ORDER_ZYX 0
#define ORDER_ZXY 1
#define ORDER_YXZ 2
#define ORDER_YZX 3
#define ORDER_XYZ 4
#define ORDER_XZY 5

/// Class representing a 4 by 4 Matrix (Translation + Rotation + Scale)
class Transform {
protected:
    //Local space information
    glm::vec3 m_translate;
    glm::vec3 m_rot; //In degrees
    glm::vec3 m_scale;
    //Global space information concatenate in matrix
    glm::mat4 m_matrix;
    //Flag Up to date
    bool m_up_to_date{};
    int m_order_rotation{};

    /**
     * Compute a matrix with given values
     * @param translate
     * @param rot
     * @param scale
     * @param order_rotation
     * @return matrix
     */
    static glm::mat4
    local_get_matrix_with_values(glm::vec3 translate, glm::vec3 rot, glm::vec3 scale, int order_rotation);

    /**
     * Compute the matrix of the Transform without saving it
     * @return matrix
     */
    glm::mat4 local_get_matrix();

    /**
     * Apply the Transform to a vec3 (General method)
     * @param v
     * @param with_translation
     * @param with_normalization
     * @return vector
     */
    glm::vec3 apply_to_vec3(glm::vec3 &v, bool with_translation, bool with_normalization);

    /**
     * Apply the Transform to a list of vec3 (General method)
     * @param vects
     * @param with_translation
     * @param with_normalization
     */
    void apply_to_vector_of_vec3(std::vector<glm::vec3> *vects, bool with_translation,
                                 bool with_normalization);   //General method
public:

    /**
     * Constructor of the Transform
     * @param translation
     * @param rotation
     * @param scale
     * @param order_rotation
     */
    explicit Transform(glm::vec3 translation = {0.0f, 0.0f, 0.0f}, glm::vec3 rotation = {0.0f, 0.0f, 0.0f},
                       glm::vec3 scale = {1.0f, 1.0f, 1.0f}, int order_rotation = ORDER_YXZ);

    /**
     * Init method also used to reset te Transform
     * @param translation
     * @param rotation
     * @param scale
     * @param order_rotation
     */
    void init(glm::vec3 translation = {0.0f, 0.0f, 0.0f}, glm::vec3 rotation = {0.0f, 0.0f, 0.0f},
              glm::vec3 scale = {1.0f, 1.0f, 1.0f}, int order_rotation = ORDER_YXZ);

    /// Compute the matrix of the Transform
    void compute();

    /**
     * Compute linear interpolation given an other Transform
     * @param transf
     * @param k
     * @return matrix
     */
    glm::mat4 compute_lerp_with_transform(Transform transf, float k);

    /**
     * Setter of the order of rotation
     * @param order_rotation
     */
    void set_order_rotation(int order_rotation);

    /**
     * Setter of the translation
     * @param new_translation
     */
    void set_translation(const glm::vec3 &new_translation);

    /**
     * Setter of the rotation
     * @param new_rotation
     */
    void set_rotation(const glm::vec3 &new_rotation);

    /// Setter of the scale
    void set_scale(const glm::vec3 &new_scale);

    /**
     * Setter of the matrix
     * @param new_matrix
     */
    void set_matrix(const glm::mat4 &new_matrix);

    /**
     * Getter of the translation
     * @return translation
     */
    const glm::vec3 &get_translation();

    /**
     * Getter of the scale
     * @return scale
     */
    const glm::vec3 &get_scale();

    /**
     * Getter of the rotation
     * @return rotation
     */
    const glm::vec3 &get_rotation();

    /**
     * Getter of the matrix
     * @return matrix
     */
    const glm::mat4 &get_matrix();

    /**
     * Getter of the flag up_to_date
     * @return up_to_date
     */
    bool is_up_to_date() const;

    /**
     * Apply the Transform to a point
     * @param v
     * @return point
     */
    glm::vec3 apply_to_point(glm::vec3 &v);

    /**
     * Apply the Transform to a vector
     * @param v
     * @return vector
     */
    glm::vec3 apply_to_vector(glm::vec3 &v);

    /**
     * Apply the Transform to a versor
     * @param v
     * @return versor
     */
    glm::vec3 apply_to_versor(glm::vec3 &v);

    /**
     * Apply the Transform to a list of point
     * @param points
     */
    void apply_to_vector_of_point(std::vector<glm::vec3> *points);

    /**
     * Apply the Transform to a list of vector
     * @param vectors
     */
    void apply_to_vector_of_vector(std::vector<glm::vec3> *vectors);

    /**
     * Apply the Transform to a list of versor
     * @param versors
     */
    void apply_to_vector_of_versor(std::vector<glm::vec3> *versors);

    /**
     * Operator equal
     * @param trsf1
     * @param trsf2
     * @return is_equal
     */
    friend bool operator==(const Transform &trsf1, const Transform &trsf2);

    /**
     * Operator inequal
     * @param trsf1
     * @param trsf2
     * @return is_inequal
     */
    friend bool operator!=(const Transform &trsf1, const Transform &trsf2);
};

#endif //GAME_ENGINE_TRANSFORM_HPP
