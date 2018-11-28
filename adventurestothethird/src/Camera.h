#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    glm::mat4 viewMatrix();
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
    void moveForward();
    void moveBackwards();
    void mouse(float mouseX, float mouseY);
    glm::vec3& position();
    float& yaw();
    float& pitch();
private:
    glm::vec3 m_position;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_front;
    float m_roll;
    float m_yaw; // left / right
    float m_pitch; // up / down
};
