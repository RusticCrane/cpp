#include <fstream>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <PerlinNoise/PerlinNoise.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"

#define WORLD_WIDTH 32
#define WORLD_HEIGHT 32

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

static constexpr float cubeVertices[] = {
	// positions         // normals         // texture coords
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
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


	Camera cam;
	cam.position().x = 16.0f;
	cam.position().y = 20.0f;
	cam.position().z = 16.0f;

	glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);

	shader.use();

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	int width, height;
	unsigned char* data = stbi_load("res/textures/grass.png", &width, &height, nullptr, 4);

	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE0);
	// TERRAIN START

	bool terrain[WORLD_WIDTH][WORLD_HEIGHT][WORLD_WIDTH];

	const siv::PerlinNoise perlin(
		std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now().time_since_epoch()).count());

	for (int x = 0; x < WORLD_WIDTH; ++x)
	{
		for (int z = 0; z < WORLD_WIDTH; ++z)
		{
			for (int y = 0; y < WORLD_HEIGHT; ++y)
			{
				terrain[x][y][z] = perlin.noise0_1(x / (WORLD_WIDTH + 1.0f), y / (WORLD_HEIGHT + 1.0f),
				                                   z / (WORLD_WIDTH + 1.0f)) > 0.5f;
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
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), cam.position().x, cam.position().y,
		            cam.position().z);

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
						glDrawArrays(GL_TRIANGLES, 0, 36);
					}
				}
			}
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
