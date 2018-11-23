#version 330 core

in vec3 inPos;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	color = (inPos + 0.8) / 1.6; 
	gl_Position = projection * view * model * vec4(inPos, 1.0);
//	gl_Position = model * view * projection * vec4(inPos, 1.0);
}