//
// Created by mariusjenin on 04/03/2022.
//

#include "CameraNodeSG.hpp"

#include <utility>

using namespace scene_graph;

void CameraNodeSG::update_view_mat() {
    glm::mat4 mat = get_matrix_recursive_intern();
    Transform trsf_tmp = Transform();
    trsf_tmp.set_matrix(mat);
    glm::vec3 camera_init_position = CAMERA_INIT_POSITION;
    glm::vec3 camera_init_forward = CAMERA_INIT_FORWARD;
    glm::vec3 camera_init_up = CAMERA_INIT_UP;
    glm::vec3 eye_camera = trsf_tmp.apply_to_point(camera_init_position);
    glm::vec3 dir_camera = trsf_tmp.apply_to_versor(camera_init_forward);
    glm::vec3 up_camera = trsf_tmp.apply_to_versor(camera_init_up);

    glm::mat4 view_mat = lookAt(
            eye_camera,
            eye_camera + dir_camera,
            up_camera
    );
    glUniformMatrix4fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::VIEW_MAT_LOC_NAME), 1,
                       GL_FALSE, &view_mat[0][0]);
}

void CameraNodeSG::update_view_pos() {
    Transform trsf_tmp = Transform();
    trsf_tmp.set_matrix(get_matrix_recursive_intern());
    glm::vec3 position_tmp = glm::vec3(0, 0, 0);
    position_tmp = trsf_tmp.apply_to_point(position_tmp);
    glUniform3fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::VIEW_POS_LOC_NAME), 1,
                 &position_tmp[0]);
}

CameraNodeSG::CameraNodeSG(Shaders *shaders, ElementSG *parent, std::string name) : NodeSG(shaders, parent,
                                                                                           std::move(name)) {

}
