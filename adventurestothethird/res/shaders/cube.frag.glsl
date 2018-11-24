#version 330 core

in vec3 color;

out vec4 fragCol;

void main() 
{
	fragCol = vec4(color, 0.0);
//	fragCol = vec4(0.067, 0.485, 0.07, 0.0);
}