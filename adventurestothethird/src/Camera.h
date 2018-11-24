#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#define SPEED 1.0f

class Camera
{
public:
	Camera() : m_position(glm::vec3(0.0f)), m_roll(0.0f), m_yaw(0.0f), m_pitch(0.0f)
	{
	}

	glm::mat4 viewMatrix()
	{
		float pitch = glm::radians(m_pitch);
		float yaw = glm::radians(m_yaw);
		m_front.x = glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
		m_front.y = glm::sin(glm::radians(m_pitch));
		m_front.z = glm::cos(glm::radians(m_pitch)) * glm::sin(glm::radians(m_yaw));
		m_front = glm::normalize(m_front);

		m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_up = glm::normalize(glm::cross(m_right, m_front));
		return glm::lookAt(m_position, m_position + m_front, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void moveUp()
	{
		m_position.y += SPEED;
	}

	void moveDown()
	{
		m_position.y -= SPEED;
	}

	void moveRight()
	{
		m_position += m_right * SPEED;
	}
	void moveLeft()
	{
		m_position -= m_right * SPEED;
	}

	void moveForward()
	{
		m_position += m_front * SPEED;
	}

	void moveBackwards()
	{
		m_position -= m_front * SPEED;
	}

	void mouse(float mouseX, float mouseY)
	{
		const float MOUSE_SPEED = 0.25f;
		m_yaw -= mouseX * MOUSE_SPEED;
		m_pitch += mouseY * MOUSE_SPEED;
	}

	glm::vec3& position()
	{
		return m_position;
	}

	float& yaw()
	{
		return m_yaw;
	}

	float& pitch()
	{
		return m_pitch;
	}

private:
	glm::vec3 m_position;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_front;
	float m_roll;
	float m_yaw; // left / right
	float m_pitch; // up / down
};
