#include <fstream>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

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

	unsigned int shaderProgram;

	Shader shader("res/shaders/cube.vert.glsl", "res/shaders/cube.frag.glsl");

	float data[] = {
		-0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.8f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.8f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned int indexData[2][3] = {
		{
			0, 1, 2
		},
		{
			2, 3, 0
		}
	};

	unsigned int vao[2], vbo[2], ibo[2];
	glGenVertexArrays(2, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(2, ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData[0]), indexData[0], GL_STATIC_DRAW);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData[1]), indexData[1], GL_STATIC_DRAW);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glBindVertexArray(vao[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(vao[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
