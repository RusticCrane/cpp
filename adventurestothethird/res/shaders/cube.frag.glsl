#version 330 core

in vec3 normal;
//in vec2 texPos;

in vec3 fragPos;

out vec4 fragCol;

//uniform sampler2D tex;

uniform vec3 viewPos;

void main() 
{
	vec4 lightColor = vec4(1.0, 1.0, 1.0, 0.0);

	float ambientStrength = 0.3;
	vec4 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(vec3(100.0, 200.0, 100.0) - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * lightColor;

	float specularStrength = 0.5;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec4 specular = specularStrength * spec * lightColor;

	fragCol = (ambient + diffuse + specular) * vec4(1.0, 0.2, 0.2, 0.0); // * texture(tex, texPos);
}