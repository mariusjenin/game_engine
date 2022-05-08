//
// Created by mariusjenin on 25/04/2022.
//

#include "NodeGameSG.hpp"

#include <src/utils/printer.hpp>
#include <utility>

using namespace scene_graph;
using namespace physics;

void NodeGameSG::draw(glm::vec3 pos_camera) {

    if (m_drawable) {
        load_model_matrices();
        load_uniforms();

        if (m_see_both_face) {
            glDisable(GL_CULL_FACE);
        } else {
            glEnable(GL_CULL_FACE);
        }

        if (m_has_material) {
            m_material->load_in_shader();
            for (auto mesh: m_meshes) {
                mesh->update_mesh(get_distance_from(pos_camera, pos_camera));
                VAODataManager::bind_vao(mesh->get_vao_id());
                VAODataManager::draw(mesh->get_ebo_triangle_indices_id(), (long) mesh->get_triangle_indices().size());
            }
        }
        if (m_debug_rendering) {
            refresh_bb(pos_camera);
            glUniform1i(
                    m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::DEBUG_RENDERING_LOC_NAME),
                    true);
            glUniform3fv(
                    m_shaders->get_shader_data_manager()->get_location(
                            ShadersDataManager::DEBUG_RENDERING_COLOR_LOC_NAME),
                    1, &m_color_rendering[0]);
            VAODataManager::draw_verticies_debug(m_bb->to_vertices());
            glUniform1i(
                    m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::DEBUG_RENDERING_LOC_NAME),
                    false);
        }

        ElementSG::draw(pos_camera);

        ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
        glUniform1i(shader_data_manager->get_location(ShadersDataManager::IS_NODE_ON_TOP_LOC_NAME), false);
    }
}


void NodeGameSG::set_meshes(std::vector<Mesh *> meshes) {
    m_meshes = std::move(meshes);
    m_meshes_dirty = true;
}

const std::vector<Mesh *> &NodeGameSG::get_meshes() const {
    return m_meshes;
}

void NodeGameSG::set_material(Material *material) {
    m_material = material;
    m_has_material = true;
}

void NodeGameSG::set_see_both_face(bool see_both_face) {
    m_see_both_face = see_both_face;
}


glm::vec3 NodeGameSG::get_center(glm::vec3 pos_camera) {
    glm::vec3 center = {0, 0, 0};
    for (auto mesh: m_meshes) {
        mesh->update_mesh(get_distance_from(pos_camera, pos_camera));
        center += mesh->get_center();
    }
    center /= m_meshes.size();
    return center;
}

std::pair<glm::vec3, glm::vec3> NodeGameSG::get_aabb(glm::vec3 pos_camera) {
    std::pair<glm::vec3, glm::vec3> bb;
    bb.first = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    bb.second = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    Transform trsf = Transform();
    trsf.set_matrix(get_matrix_recursive_local());
    for (auto mesh: m_meshes) {

        mesh->update_mesh(glm::distance(get_position_in_world(mesh->get_center()), pos_camera));
        std::pair<glm::vec3, glm::vec3> bb_curr;
        AABB aabb = AABB();
        aabb.compute(mesh->get_vertex_positions());
        bb_curr.first = aabb.get_min();
        bb_curr.second = aabb.get_max();
        bb_curr.first = trsf.apply_to_point(bb_curr.first);
        bb_curr.second = trsf.apply_to_point(bb_curr.second);
        for (int i = 0; i < 3; i++) {
            if (bb_curr.first[i] > bb_curr.second[i]) {
                float tmp = bb_curr.first[i];
                bb_curr.first[i] = bb_curr.second[i];
                bb_curr.second[i] = tmp;
            }
        }
        if (bb_curr.first[0] < bb.first[0]) bb.first[0] = bb_curr.first[0];
        if (bb_curr.first[1] < bb.first[1]) bb.first[1] = bb_curr.first[1];
        if (bb_curr.first[2] < bb.first[2]) bb.first[2] = bb_curr.first[2];
        if (bb_curr.second[0] > bb.second[0]) bb.second[0] = bb_curr.second[0];
        if (bb_curr.second[1] > bb.second[1]) bb.second[1] = bb_curr.second[1];
        if (bb_curr.second[2] > bb.second[2]) bb.second[2] = bb_curr.second[2];
    }

    return bb;
}


float NodeGameSG::get_distance_from(glm::vec3 cam_position, glm::vec3 position) {

    std::pair<glm::vec3, glm::vec3> bb = get_aabb(cam_position);
    glm::vec3 min_vals;
    float curr_val;
    for (int i = 0; i < 3; i++) {
        min_vals[i] = FLT_MAX;
        if (position[i] > bb.first[i] && position[i] < bb.second[i]) {
            min_vals[i] = 0;
        } else {
            curr_val = std::abs(bb.first[i] - position[i]);
            if (curr_val < min_vals[i]) min_vals[i] = curr_val;
            curr_val = std::abs(bb.second[i] - position[i]);
            if (curr_val < min_vals[i]) min_vals[i] = curr_val;
        }
    }
    return std::max(min_vals[0], std::max(min_vals[1], min_vals[2]));
}

NodeGameSG::NodeGameSG(Shaders *shaders, ElementSG *parent, BB_TYPE bb_type) : NodeSG(shaders, parent) {
    m_see_both_face = false;
    m_has_material = false;
    m_light = nullptr;
    m_bb_type = bb_type;
    m_meshes_dirty = true;
    m_drawable = true;
}

void NodeGameSG::set_light(Light *light) {
    m_light = light;
}

void NodeGameSG::set_drawable(bool drawable) {
    m_drawable = drawable;
}

void NodeGameSG::remove_light() {
    m_light = nullptr;
}

bool NodeGameSG::has_light() {
    return m_light != nullptr;
}


void NodeGameSG::update_view_mat(glm::vec3 forward) {
    glm::mat4 mat = get_matrix_recursive_local();
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

void NodeGameSG::update_view_pos() {
    Transform trsf_tmp = Transform();
    trsf_tmp.set_matrix(get_matrix_recursive_local());
    glm::vec3 position_tmp = glm::vec3(0, 0, 0);
    position_tmp = trsf_tmp.apply_to_point(position_tmp);
    glUniform3fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::VIEW_POS_LOC_NAME), 1,
                 &position_tmp[0]);
}

LightShader NodeGameSG::generate_light_struct() {
    LightShader light_struct{};
    m_light->to_light_shader(&light_struct);

    if (m_light->positionned_light()) {
        Transform light_trsf_tmp = Transform();
        light_trsf_tmp.set_matrix(get_matrix_recursive_local());
        glm::vec3 light_position_tmp = {0, 0, 0};
        light_position_tmp = light_trsf_tmp.apply_to_point(light_position_tmp);
        light_struct.position = glsl_vec3(light_position_tmp);
    }
    return light_struct;
}

bool NodeGameSG::refresh_bb_aux(glm::vec3 pos_camera, bool force_compute) {
    std::vector<BoundingBox *> bbs = {};

    bool has_to_be_computed = force_compute || m_meshes_dirty || m_children_dirty || true;

    for (auto child: m_children) {
        if (child->is_node_game()) {

            auto *node = (NodeGameSG *) child;
            if (node->has_children() || node->has_meshes()) {
                has_to_be_computed = node->refresh_bb_recursive(pos_camera) || has_to_be_computed;
            }
            //TODO mesh null getbb
            bbs.push_back(node->get_bb());
        }
    }
    if (has_to_be_computed) {
        for (auto mesh: m_meshes) {

            mesh->update_mesh(glm::distance(get_position_in_world(mesh->get_center()), pos_camera));
            bbs.push_back(mesh->get_bb());

        }
        m_meshes_dirty = false;
        m_bb = BBFactory::generate_bb(m_bb_type);
        m_bb->compute(bbs);
    }

    return has_to_be_computed;
}

bool NodeGameSG::refresh_bb(glm::vec3 pos_camera) {
    auto *dirty = new TransformDirty(false);
    glm::mat4 mat = get_matrix_recursive_local(dirty);
    bool has_computed = refresh_bb_aux(pos_camera, dirty->has_dirty());
    if (has_computed) m_bb->apply_transform(mat);
    return has_computed;
}

bool NodeGameSG::refresh_bb_recursive(glm::vec3 pos_camera) {
    auto *dirty = new TransformDirty(false);
    dirty->logic_or(*m_trsf->is_dirty());
    dirty->logic_or(*m_local_trsf->is_dirty());
    bool has_computed = refresh_bb_aux(pos_camera, dirty->has_dirty());
    if (has_computed) m_bb->apply_transform(m_trsf->get_matrix() * m_local_trsf->get_matrix());
    return has_computed;
}

BoundingBox *NodeGameSG::get_bb() {
    return m_bb;
}

bool NodeGameSG::is_node_game() {
    return true;
}

RigidBodyVolume *NodeGameSG::get_rigid_body() const {
    return m_rigid_body;
}

void NodeGameSG::set_rigid_body(RigidBodyVolume *rigid_body) {
    m_rigid_body = rigid_body;
}

void NodeGameSG::set_debug_rendering(bool dr, glm::vec3 color_rendering) {
    m_debug_rendering = dr;
    m_color_rendering = color_rendering;
}

bool NodeGameSG::has_meshes() const {
    return !m_meshes.empty();
}

Material *NodeGameSG::get_material() const {
    return m_material;
}
