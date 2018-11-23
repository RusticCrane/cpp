#include <fstream>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	glViewport(0, 0, 640, 480);
	glEnable(GL_DEPTH_TEST);

	Shader shader("res/shaders/cube.vert.glsl", "res/shaders/cube.frag.glsl");

	unsigned int modelLocation = glGetUniformLocation(shader.getId(), "model");
	unsigned int viewLocation = glGetUniformLocation(shader.getId(), "view");
	unsigned int projectionLocation = glGetUniformLocation(shader.getId(), "projection");

	glm::mat4 model(1.0f);
	float rotation = 0.0f;
	//model = glm::translate(model, glm::vec3(0.2f, 0.2f, 0.0f));
	

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::perspective(45.0f, 640.0f / 480.0f, 0.01f, 100.0f);

	shader.use();
	
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
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

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			rotation -= 4.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			rotation += 4.0f;
		}

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		model = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
