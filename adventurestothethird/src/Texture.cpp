#include "Texture.h"

#include <glad/glad.h>

#include "stb_image.h"

Texture::Texture(const std::string& path)
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	int width, height;
	unsigned char* data = stbi_load("res/textures/grass.png", &width, &height, nullptr, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		throw std::exception("Could not load image");
	}
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}