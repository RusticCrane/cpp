#include <fstream>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

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

	Shader shader("res/shaders/cube.vert.glsl", "res/shaders/cube.frag.glsl");

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
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
