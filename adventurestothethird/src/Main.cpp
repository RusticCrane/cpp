#include <fstream>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <PerlinNoise/PerlinNoise.hpp>

#include "Shader.h"
#include "Camera.h"

#define WORLD_WIDTH 32
#define WORLD_HEIGHT 24

static constexpr float vertices[] = {
	-0.5f, -0.5f, 0.5f, // FRONT BOTTOM LEFT
	-0.5f, 0.5f, 0.5f, // FRONT TOP LEFT
	0.5f, 0.5f, 0.5f, // FRONT TOP RIGHT
	0.5f, -0.5f, 0.5f, // FRONT BOTTOM RIGHT
	-0.5f, -0.5f, -0.5f, // BACK BOTTOM LEFT
	-0.5f, 0.5f, -0.5f, // BACK TOP LEFT
	0.5f, 0.5f, -0.5f, // BACK TOP RIGHT
	0.5f, -0.5f, -0.5f, // BACK BOTTOM RIGHT
};

static constexpr unsigned int indices[] = {
	0, 1, 2, 2, 3, 0, // FRONT
	1, 5, 6, 6, 2, 1, // TOP
	3, 2, 6, 6, 7, 3, // RIGHT
	4, 7, 3, 3, 0, 4, // BOTTOM
	4, 0, 1, 1, 5, 4, // LEFT
	6, 7, 4, 4, 5, 6, // BACK
};

int main(int argc, char** argv)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);

	Shader shader("res/shaders/cube.vert.glsl", "res/shaders/cube.frag.glsl");

	unsigned int modelLocation = glGetUniformLocation(shader.getId(), "model");
	unsigned int viewLocation = glGetUniformLocation(shader.getId(), "view");
	unsigned int projectionLocation = glGetUniformLocation(shader.getId(), "projection");

	glm::mat4 model(1.0f);
	float rotation = 0.0f;
	//model = glm::translate(model, glm::vec3(0.2f, 0.2f, 0.0f));


	Camera cam;
	cam.position().x = 16.0f;
	cam.position().y = 20.0f;
	cam.position().z = 16.0f;
	//glm::mat4 view = glm::lookAt(glm::vec3(5.0f, 20.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);

	shader.use();

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// TERRAIN START

	bool terrain[WORLD_WIDTH][WORLD_HEIGHT][WORLD_WIDTH];

	const siv::PerlinNoise perlin(24);

	for (int x = 0; x < WORLD_WIDTH; ++x)
	{
		for (int z = 0; z < WORLD_WIDTH; ++z)
		{
			int height = static_cast<int>(perlin.noise0_1(x / (WORLD_WIDTH + 1.0f) * 2.3f,
			                                              z / (WORLD_WIDTH + 1.0f) * 2.3f) * WORLD_HEIGHT);
			for (int y = 0; y < WORLD_HEIGHT; ++y)
			{
				if (y < height)
				{
					terrain[x][y][z] = true;
				}
				else
				{
					terrain[x][y][z] = false;
				}
			}
		}
	}

	// TERRAIN END

	double mX, mY;
	glfwGetCursorPos(window, &mX, &mY);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		float deltaX = mX - mouseX;
		float deltaY = mY - mouseY;
		cam.mouse(deltaX, deltaY);
		mX = mouseX;
		mY = mouseY;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cam.moveLeft();
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cam.moveRight();
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			cam.moveUp();
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			cam.moveDown();
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cam.moveForward();
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cam.moveBackwards();
		}

		std::cout << "Position(" << cam.position().x << ", " << cam.position().y << ", " << cam.position().z << ") Yaw("
			<< cam.yaw() << ") Pitch(" << cam.pitch() << ")" << std::endl;

		shader.use();
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(cam.viewMatrix()));

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int x = 0; x < WORLD_WIDTH; ++x)
		{
			for (int y = 0; y < WORLD_HEIGHT; ++y)
			{
				for (int z = 0; z < WORLD_WIDTH; ++z)
				{
					if (terrain[x][y][z])
					{
						glm::mat4 m = glm::translate(glm::mat4(1.0f),
						                             glm::vec3(static_cast<float>(x), static_cast<float>(y),
						                                       static_cast<float>(z)));
						glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m));
						glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
					}
				}
			}
		}
		//model = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
