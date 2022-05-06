#include "MouseView.hpp"

//Static members must be defined
MouseView* MouseView::instance = nullptr;

MouseView::MouseView(float width, float height): m_first_mouse(true), m_yaw(0.f), m_pitch(0.0f), m_front(glm::vec3(0,0,-1)){
    m_last_x = width / 2.f;
    m_last_y = height / 2.f;
}

MouseView* MouseView::get_instance(){
    
    if(MouseView::instance == NULL)
        MouseView::instance = new MouseView(10.f, 74.f);
    return MouseView::instance;
}

void MouseView::process(GLFWwindow * window, double x, double y){
    if(m_first_mouse){
        m_first_mouse = false;
        m_last_x = x;
        m_last_y = y;
    }
    
    float x_offset = x - m_last_x;
    float y_offset = m_last_y - y;

    m_last_x = x;
    m_last_y = y;

    float sensivity = 0.1f;

    x_offset *= sensivity;
    y_offset *= sensivity;

    m_yaw += x_offset;
    m_pitch += y_offset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    // glm::vec3 y_rot(0, cam_yaw, 0);   
}

void MouseView::process_mouse(GLFWwindow * window, double x, double y){
    get_instance()->process(window, x, y);
}

glm::vec3 MouseView::get_front(){
    return m_front;
}

float MouseView::get_pitch(){
    return m_pitch;
}

float MouseView::get_yaw(){
    return m_yaw;
}





