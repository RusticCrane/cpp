#pragma once

#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#define SPEED 0.006f

Camera::Camera() : m_position(glm::vec3(0.0f)), m_roll(0.0f), m_yaw(0.0f), m_pitch(0.0f)
{
}

glm::mat4 Camera::viewMatrix()
{
	float pitch = glm::radians(m_pitch);
	float yaw = glm::radians(m_yaw);
	m_front.x = glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
	m_front.y = glm::sin(glm::radians(m_pitch));
	m_front.z = glm::cos(glm::radians(m_pitch)) * glm::sin(glm::radians(m_yaw));
	m_front = glm::normalize(m_front);

	m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
	// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));
	return glm::lookAt(m_position, m_position + m_front, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::moveUp()
{
	m_position.y += SPEED;
}

void Camera::moveDown()
{
	m_position.y -= SPEED;
}

void Camera::moveRight()
{
	m_position += m_right * SPEED;
}

void Camera::moveLeft()
{
	m_position -= m_right * SPEED;
}

void Camera::moveForward()
{
	m_position += m_front * SPEED;
}

void Camera::moveBackwards()
{
	m_position -= m_front * SPEED;
}

void Camera::mouse(float mouseX, float mouseY)
{
	const float MOUSE_SPEED = 0.25f;
	m_yaw -= mouseX * MOUSE_SPEED;
	m_pitch += mouseY * MOUSE_SPEED;
	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	else if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}
}

glm::vec3& Camera::position()
{
	return m_position;
}

float& Camera::yaw()
{
	return m_yaw;
}

float& Camera::pitch()
{
	return m_pitch;
}
