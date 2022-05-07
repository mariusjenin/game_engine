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
        void process(GLFWwindow*, double, double);

    public:

        //Getter of the unique instance of this class.
        static MouseView* get_instance();
        
        // Static method called in main mouse callback.
        static void process_mouse(GLFWwindow *, double, double);

        //Getter of m_front vec3.
        glm::vec3 get_front();

        float get_pitch();
        float get_yaw();

        

};
#endif