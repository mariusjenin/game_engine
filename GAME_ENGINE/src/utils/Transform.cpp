//
// Created by marius.jenin@etu.umontpellier.fr on 17/02/2022.
//

#include "Transform.hpp"

Transform::Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, int order_rotation) {
    init(translation, rotation, scale, order_rotation);
}

glm::mat4
Transform::local_get_matrix_with_values(const glm::vec3 translate, const glm::vec3 rot, const glm::vec3 scale,
                                        int order_rotation) {
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
                                             glm::radians(rot.x),
                                             glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
                                             glm::radians(rot.y),
                                             glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
                                             glm::radians(rot.z),
                                             glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotationMatrix;
    switch (order_rotation) {
        case ORDER_ZYX:
            rotationMatrix = transformZ * transformY * transformX;
            break;
        case ORDER_ZXY:
            rotationMatrix = transformZ * transformX * transformY;
            break;
        case ORDER_YXZ:
            rotationMatrix = transformY * transformX * transformZ;
            break;
        case ORDER_YZX:
            rotationMatrix = transformY * transformZ * transformX;
            break;
        case ORDER_XYZ:
            rotationMatrix = transformX * transformY * transformZ;
            break;
        case ORDER_XZY:
            rotationMatrix = transformX * transformZ * transformY;
            break;
        default:
            throw std::runtime_error("Bad order of rotation");
    }
    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), translate) *
           rotationMatrix *
           glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 Transform::local_get_matrix() {
    return local_get_matrix_with_values(m_translate, m_rot, m_scale, m_order_rotation);
}

void Transform::compute() {
    if (!m_up_to_date) {
        m_matrix = local_get_matrix();
        m_up_to_date = true;
    }
}


glm::mat4 Transform::compute_lerp_with_transform(const Transform transf, float k) {
    const glm::vec3 t = m_translate * k + transf.m_translate * (1 - k);
    const glm::vec3 s = m_scale * k + transf.m_scale * (1 - k);
    const glm::vec3 r = m_rot * k + transf.m_rot * (1 - k);
    if (m_order_rotation != transf.m_order_rotation)
        throw std::runtime_error("Transform don't have the same order of rotation");
    return local_get_matrix_with_values(t, r, s, m_order_rotation);
}

const glm::vec3 &Transform::get_translation() {
    return m_translate;
}

const glm::vec3 &Transform::get_rotation() {
    return m_rot;
}

const glm::vec3 &Transform::get_scale() {
    return m_scale;
}


const glm::mat4 &Transform::get_matrix() {
    return m_matrix;
}

bool Transform::is_up_to_date() const {
    return m_up_to_date;
}

void Transform::set_translation(const glm::vec3 &new_translation) {
    m_up_to_date = m_up_to_date && new_translation == m_translate;
    m_translate = new_translation;
}

void Transform::set_rotation(const glm::vec3 &new_rotation) {
    m_up_to_date = m_up_to_date && new_rotation == m_rot;
    m_rot = new_rotation;
}

void Transform::set_scale(const glm::vec3 &new_scale) {
    m_up_to_date = m_up_to_date && new_scale == m_scale;
    m_scale = new_scale;
}

glm::vec3 Transform::apply_to_vec3(glm::vec3 &v, bool with_translation, bool with_normalization) {
    float w = with_translation ? 1.f : 0.f;
    glm::vec3 u = glm::vec3(m_matrix * glm::vec4(v, w));
    if (with_normalization) glm::normalize(u);
    return u;
}

glm::vec3 Transform::apply_to_point(glm::vec3 &v) {
    return apply_to_vec3(v, true, false);
}

glm::vec3 Transform::apply_to_vector(glm::vec3 &v) {
    return apply_to_vec3(v, false, false);
}

glm::vec3 Transform::apply_to_versor(glm::vec3 &v) {
    return apply_to_vec3(v, false, true);
}

void Transform::apply_to_vector_of_vec3(std::vector<glm::vec3> *vects, bool with_translation, bool with_normalization) {
    int size_vector = vects->size();
    for (int i = 0; i < size_vector; i++) {
        vects->at(i) = apply_to_vec3(vects->at(i), with_translation, with_normalization);
    }
}

void Transform::apply_to_vector_of_point(std::vector<glm::vec3> *points) {
    apply_to_vector_of_vec3(points, true, false);
}

void Transform::apply_to_vector_of_vector(std::vector<glm::vec3> *vectors) {
    apply_to_vector_of_vec3(vectors, false, false);
}

void Transform::apply_to_vector_of_versor(std::vector<glm::vec3> *versors) {
    apply_to_vector_of_vec3(versors, false, true);
}

void Transform::init(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, int order_rotation) {
    m_translate = translation;
    m_rot = rotation;
    m_scale = scale;
    m_matrix = glm::mat4(1.0f);
    m_up_to_date = false;
    m_order_rotation = order_rotation;
}


bool operator==(const Transform &trsf1, const Transform &trsf2) {
    return trsf1.m_translate == trsf2.m_translate &&
           trsf1.m_rot == trsf2.m_rot &&
           trsf1.m_scale == trsf2.m_scale &&
           trsf1.m_matrix == trsf2.m_matrix &&
           trsf1.m_up_to_date == trsf2.m_up_to_date;
}

bool operator!=(const Transform &trsf1, const Transform &trsf2) {
    return !(trsf1 == trsf2);
}

void Transform::set_matrix(const glm::mat4 &new_matrix) {
    m_up_to_date = m_up_to_date && new_matrix == m_matrix;
    m_matrix = new_matrix;
}

void Transform::set_order_rotation(int order_rotation) {
    m_up_to_date = m_up_to_date && order_rotation == m_order_rotation;
    m_order_rotation = order_rotation;
}
