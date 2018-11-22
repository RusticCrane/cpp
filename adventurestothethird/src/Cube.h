#pragma once
#include <glad/glad.h>

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

class Mesh
{
public:
	Mesh(float* vertices, unsigned int numberOfVertices, unsigned int* indices, unsigned int numberOfIndices)
	{
		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfVertices, vertices, GL_STATIC_DRAW);
		//glVertexAttribPointer(0, )

		unsigned int ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numberOfIndices, indices, GL_STATIC_DRAW);
	}
private:
	unsigned int m_size;
};
