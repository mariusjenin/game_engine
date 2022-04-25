//
// Created by mariusjenin on 05/03/2022.
//

#ifndef GAME_ENGINE_SOLARSYSTEM_HPP
#define GAME_ENGINE_SOLARSYSTEM_HPP

#include "Scene.hpp"
#include <src/material/MaterialColor.hpp>
#include <src/material/MaterialTexture.hpp>
#include <src/light/PositionLight.hpp>
#include <src/mesh/LODMesh.hpp>

namespace scene {
    /// Scene with the sun, the earth, the moon end the sky
    class SolarSystem : public Scene {
        static const int NOT_STAR_ID = -1;
        static const int SUN_ID = 0;
        static const int EARTH_ID = 1;
        static const int MOON_ID = 2;

        static constexpr const char *SUN_LOC_NAME = "SUN_ID";
        static constexpr const char *EARTH_LOC_NAME = "EARTH_ID";
        static constexpr const char *MOON_LOC_NAME = "MOON_ID";
        static constexpr const char *HM_SUN_LOC_NAME = "hm_sun";
        static constexpr const char *HM_EARTH_LOC_NAME = "hm_earth";
        static constexpr const char *HM_MOON_LOC_NAME = "hm_moon";
        static constexpr const char *ALWAYS_ENLIGHTENED_LOC_NAME = "always_enlightened";
        static constexpr const char *STAR_ID_LOC_NAME = "star_id";
    private:
        float m_speed_anime{};
        float m_timing;
        NodeGameSG *m_sun{};
        NodeGameSG *m_sky{};
        NodeGameSG *m_earth1{};
        NodeGameSG *m_earth2{};
        NodeGameSG *m_earth3{};
        NodeGameSG *m_moon1{};
        NodeGameSG *m_moon2{};
        NodeGameSG *m_moon3{};
    protected:
        void process_input(GLFWwindow *window, float delta_time) override;

        /// Load the star type to the shader
        void load_type_star_location() const;

    public:
        /**
         * Constructor of the SolarSystem
         * @param vertex_shader_path
         * @param fragment_shader_path
         */
        SolarSystem(const std::string &vertex_shader_path, const std::string &fragment_shader_path);

        void update(GLFWwindow *window, float delta_time) override;
    };
}

#endif //GAME_ENGINE_SOLARSYSTEM_HPP
