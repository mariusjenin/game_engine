//
// Created by mariusjenin on 19/03/2022.
//

#ifndef GAME_ENGINE_SCENELAND_HPP
#define GAME_ENGINE_SCENELAND_HPP


#include "Scene.hpp"
#include "../mesh/LODMesh.hpp"
#include "../material/MaterialTexture.hpp"
#include "../material/MaterialColor.hpp"
#include "src/light/DirectionLight.hpp"
#include "../utils/objloader.hpp"

namespace scene {
    /// Scene with a land and a ball on it
    class SceneLand : public Scene {
    private:
        NodeGameSG *m_ball;
    protected:
        void process_input(float delta_time) override;

    public:
        static constexpr const char *HM_LAND_LOC_NAME = "hm_land";
        static constexpr const char *HAS_HM_LOC_NAME = "has_hm";

        /**
         * Constructor of the SceneLand
         * @param window
         * @param vertex_shader_path
         * @param fragment_shader_path
         * @param mult_physics
         */
        SceneLand(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path, float mult_physics = 1.0f);
    };

}
#endif //GAME_ENGINE_SCENELAND_HPP
