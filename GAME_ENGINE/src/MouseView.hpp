#ifndef GAME_ENGINE_MOUSEVIEW_HPP
#define GAME_ENGINE_MOUSEVIEW_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

// Singleton class to implement mouse motion callback method (used to set character's sight).
class MouseView{
    private:
        float m_yaw, m_pitch;
        float m_last_x, m_last_y;
        bool m_first_mouse;
        glm::vec3 m_front;
        static MouseView* instance;
        
        //Constructor of singleton (has to be private)
        MouseView(float, float);

        //Actual method that computes sight.
        void process(GLFWwindow*, float, float);

    public:

        /**
         * Getter of the unique instance of this class.
         * @return instance
         */
        static MouseView* get_instance();

        /**
         *  Static method called in main mouse callback.
         * @param window
         * @param x
         * @param y
         */
        static void process_mouse(GLFWwindow * window, double x, double y);

        /**
         * Getter of m_front vec3.
         * @return front
         */
        glm::vec3 get_front();

        /**
         * Getter of the pitch
         * @return pitch
         */
        float get_pitch() const;

        /**
         * getter of the yaw
         * @return yaw
         */
        float get_yaw() const;

        

};
#endif