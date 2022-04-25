
#ifndef GAME_ENGINE_NODEGAMESG_H
#define GAME_ENGINE_NODEGAMESG_H


#include "NodeSG.hpp"
#include <src/material/Material.hpp>
#include <src/light/Light.hpp>
#include <src/physics/bounding_box/BoundingBox.hpp>
#include <src/physics/bounding_box/AABB.hpp>
#include "src/mesh/Mesh.hpp"
#include <src/physics/RigidBodyVolume.hpp>

using namespace light;
using namespace mesh;
using namespace material;

namespace scene_graph {
    ///NodeSG that can have \link mesh::Mesh Meshes\endlink, light and camera
    class NodeGameSG : public NodeSG {

    #define CAMERA_INIT_POSITION glm::vec3(0, 0, 0)
    #define CAMERA_INIT_FORWARD glm::vec3(0, 0, -1)
    #define CAMERA_INIT_UP glm::vec3(0, 1, 0)
    private:
        std::vector<Mesh *> m_meshes;
        Material *m_material{};
        bool m_has_material;
        bool m_see_both_face;
        Light *m_light;
        BoundingBox* m_bb{};
        BB_TYPE m_bb_type;
    public:
        /**
         * Constructor of the NodeGameSG
         * @param shaders
         * @param parent parent in the scene graph
         * @param name
         */
        NodeGameSG(Shaders *shaders, ElementSG *parent, BB_TYPE bb_type = AABB_TYPE);

        /**
         * Refresh the BoundingBox of the NodeGameSG
         * @param pos_camera
         */
        void refresh_bb(glm::vec3 pos_camera);

        /**
         * Set a light at this NodeGameSG
         * @param light
         */
        void set_light(Light *light);

        /**
         * Remove the light to this NodeGameSG
         */
        void remove_light();

        /**
         * Getter of if the NodeGameSG has a light or no
         * @return has light
         */
        bool has_light();

        /**
         * Give the \link light::LightShader LightShader\endlink that correspond to the \link light::Light Light\endlink
         * @return light_shader
         */
        LightShader generate_light_struct();

        ///Update the view matrix in the shader
        void update_view_mat();

        ///Update the view position in the shader
        void update_view_pos();

        void draw(glm::vec3 pos_camera) override;
        /**
        * Setter of the \link mesh::Mesh Mesh\endlink list of the NodeGameSG
        * @param meshes
        */
        void set_meshes(std::vector<Mesh *> meshes);

        /**
         * Setter of the material applied to the NodeGameSG
         * @param material
         */
        void set_material(Material *material);

        /**
         * Getter of the \link mesh::Mesh Mesh\endlink list of the NodeGameSG
         * @return meshes
         */
        const std::vector<Mesh *> &get_meshes() const;

        /**
        * Gives the center of the NodeGameSG (mean of each \link mesh::Mesh Mesh\endlink centers)
        * @param pos_camera
        * @return center
        */
        glm::vec3 get_center(glm::vec3 pos_camera);

        /**
         * Setter of the flag see_both_face (render both face of the \link mesh::Mesh Meshes\endlink))
         * @param see_both_face
         */
        void set_see_both_face(bool see_both_face);

        /**
         * Gives the aligned axis bounding box of the NodeGameSG (bounding box of \link mesh::Mesh Mesh\endlink bounding boxes)
         * @param pos_camera
         * @return bounding_box
         */
        std::pair<glm::vec3, glm::vec3> get_aabb(glm::vec3 pos_camera);


        /**
         * Compute the distance between the bounding box and a given position
         * @param cam_position
         * @param position
         * @return distance
         */
        float get_distance_from(glm::vec3 cam_position, glm::vec3 position);

        /**
         * Getter of the BoundingBox
         * @return bb
         */
        BoundingBox *get_bb();
    };
}

#endif //GAME_ENGINE_NODEGAMESG_H
