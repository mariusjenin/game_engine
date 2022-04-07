//
// Created by mariusjenin on 19/03/2022.
//

#ifndef GAME_ENGINE_SCENELAND_HPP
#define GAME_ENGINE_SCENELAND_HPP


#include "Scene.hpp"
#include "../mesh/LODMesh.hpp"
#include "../material/MaterialTexture.hpp"
#include "../material/MaterialColor.hpp"
#include "../light/DirectionLight.hpp"
#include "../utils/objloader.hpp"

namespace scene {
    /// Scene with a land and a ball on it
    class SceneLand : public Scene {
    private:
        NodeSG *m_ball;
    protected:
        void process_input(GLFWwindow *window, float delta_time) override;

    public:
        static constexpr const char *HM_LAND_LOC_NAME = "hm_land";
        static constexpr const char *HAS_HM_LOC_NAME = "has_hm";

        /**
         * Constructor of the SceneLand
         * @param vertex_shader_path
         * @param fragment_shader_path
         */
        SceneLand(const std::string &vertex_shader_path, const std::string &fragment_shader_path);

        ~SceneLand() override;
    };

}
#endif //GAME_ENGINE_SCENELAND_HPP
