#include <fstream>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void CreateShaderProgram(unsigned int& programIdOut);

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

	CreateShaderProgram(shaderProgram);

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

		glUseProgram(shaderProgram);
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

void CreateShaderProgram(unsigned int& programIdOut)
{
	std::string vertex;
	std::string fragment;

	std::ifstream vertexFile("res/shaders/cube.vert.glsl");
	std::ifstream fragmentFile("res/shaders/cube.frag.glsl");

	vertexFile.seekg(0, std::ios::end);
	vertex.resize(vertexFile.tellg());
	vertexFile.seekg(0);
	vertexFile.read(vertex.data(), vertex.size());

	fragmentFile.seekg(0, std::ios::end);
	fragment.resize(fragmentFile.tellg());
	fragmentFile.seekg(0);
	fragmentFile.read(fragment.data(), fragment.size());

	unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexSource = vertex.c_str();
	glShaderSource(vertexShaderId, 1, &vertexSource, nullptr);
	glCompileShader(vertexShaderId);

	int result;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		char log[512];
		glGetShaderInfoLog(vertexShaderId, 512, nullptr, log);
		std::cout << "Vertex shader error: " << log << std::endl;
	}

	unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentSource = fragment.c_str();
	glShaderSource(fragmentShaderId, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		char log[512];
		glGetShaderInfoLog(fragmentShaderId, 512, nullptr, log);
		std::cout << "Fragment shader error: " << log << std::endl;
	}

	programIdOut = glCreateProgram();
	glAttachShader(programIdOut, vertexShaderId);
	glAttachShader(programIdOut, fragmentShaderId);
	glLinkProgram(programIdOut);

	glGetProgramiv(programIdOut, GL_LINK_STATUS, &result);
	if (!result)
	{
		char log[512];
		glGetProgramInfoLog(programIdOut, 512, nullptr, log);
		std::cout << "Shader program error: " << log << std::endl;
	}
}
