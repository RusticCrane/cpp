#version 330 core

in vec3 inPos;
in vec3 inCol;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(inPos, 1.0);
	color = inCol;
}
