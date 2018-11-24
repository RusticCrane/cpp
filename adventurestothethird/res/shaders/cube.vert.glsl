#version 330 core

in vec3 inPos;
in vec3 inNormal;
in vec2 inTexPos;

out vec3 normal;
out vec2 texPos;

out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(inPos, 1.0);
	texPos = inTexPos;
	normal = inNormal;
	fragPos = vec3(model * vec4(inPos, 1.0));
}