#version 330 core

in vec3 inPos;
in vec3 inCol;

out vec3 outCol;
void main()
{
	gl_Position = vec4(inPos, 1.0);
	outCol = inCol;
}