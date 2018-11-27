#pragma once

#include <chrono>
#include <PerlinNoise/PerlinNoise.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define CHUNK_SIZE 32

static constexpr float cubeVertices[] = {
    // positions         // normals         // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, // 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, // 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, // 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, // 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, // 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, // 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 0.0f, 1.0f
};

class Chunk
{
public:
    Chunk()
    {
        const siv::PerlinNoise perlin(5);

        for (int x = 0; x < CHUNK_SIZE; ++x)
        {
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                for (int z = 0; z < CHUNK_SIZE; ++z)
                {
                    m_blocks[x][y][z] = perlin.noise0_1(x / (CHUNK_SIZE + 1.0f), y / (CHUNK_SIZE + 1.0f),
                                                        z / (CHUNK_SIZE + 1.0f)) > 0.5f;
                }
            }
        }

        std::vector<float> vertices;

        for (int x = 0; x < CHUNK_SIZE; ++x)
        {
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                for (int z = 0; z < CHUNK_SIZE; ++z)
                {
                    if (m_blocks[x][y][z])
                    {
                        // BACK
                        if (z == 0 || (z > 0 && !m_blocks[x][y][z - 1]))
                        {
                            for (int i = 0; i < 6; ++i)
                            {
                                vertices.push_back(cubeVertices[i * 6] + x);
                                vertices.push_back(cubeVertices[i * 6 + 1] + y);
                                vertices.push_back(cubeVertices[i * 6 + 2] + z);
                                vertices.push_back(cubeVertices[i * 6 + 3]);
                                vertices.push_back(cubeVertices[i * 6 + 4]);
                                vertices.push_back(cubeVertices[i * 6 + 5]);
                            }
                        }
                        //FRONT
                        if (z == CHUNK_SIZE - 1 || (z < CHUNK_SIZE - 1 && !m_blocks[x][y][z + 1]))
                        {
                            for (int i = 6; i < 12; ++i)
                            {
                                vertices.push_back(cubeVertices[i * 6] + x);
                                vertices.push_back(cubeVertices[i * 6 + 1] + y);
                                vertices.push_back(cubeVertices[i * 6 + 2] + z);
                                vertices.push_back(cubeVertices[i * 6 + 3]);
                                vertices.push_back(cubeVertices[i * 6 + 4]);
                                vertices.push_back(cubeVertices[i * 6 + 5]);
                            }
                        }
                        //RIGHT
                        if (x == 0 || (x > 0 && !m_blocks[x - 1][y][z]))
                        {
                            for (int i = 12; i < 18; ++i)
                            {
                                vertices.push_back(cubeVertices[i * 6] + x);
                                vertices.push_back(cubeVertices[i * 6 + 1] + y);
                                vertices.push_back(cubeVertices[i * 6 + 2] + z);
                                vertices.push_back(cubeVertices[i * 6 + 3]);
                                vertices.push_back(cubeVertices[i * 6 + 4]);
                                vertices.push_back(cubeVertices[i * 6 + 5]);
                            }
                        }
                        //LEFT
                        if (x == CHUNK_SIZE - 1 || (x < CHUNK_SIZE - 1 && !m_blocks[x + 1][y][z]))
                        {
                            for (int i = 18; i < 24; ++i)
                            {
                                vertices.push_back(cubeVertices[i * 6] + x);
                                vertices.push_back(cubeVertices[i * 6 + 1] + y);
                                vertices.push_back(cubeVertices[i * 6 + 2] + z);
                                vertices.push_back(cubeVertices[i * 6 + 3]);
                                vertices.push_back(cubeVertices[i * 6 + 4]);
                                vertices.push_back(cubeVertices[i * 6 + 5]);
                            }
                        }
                        //BOTTOM
                        if (y == 0 || (y > 0 && !m_blocks[x][y - 1][z]))
                        {
                            for (int i = 24; i < 30; ++i)
                            {
                                vertices.push_back(cubeVertices[i * 6] + x);
                                vertices.push_back(cubeVertices[i * 6 + 1] + y);
                                vertices.push_back(cubeVertices[i * 6 + 2] + z);
                                vertices.push_back(cubeVertices[i * 6 + 3]);
                                vertices.push_back(cubeVertices[i * 6 + 4]);
                                vertices.push_back(cubeVertices[i * 6 + 5]);
                            }
                        }
                        //TOP
                        if (y == CHUNK_SIZE - 1 || (y < CHUNK_SIZE - 1 && !m_blocks[x][y + 1][z]))
                        {
                            for (int i = 30; i < 36; ++i)
                            {
                                vertices.push_back(cubeVertices[i * 6] + x);
                                vertices.push_back(cubeVertices[i * 6 + 1] + y);
                                vertices.push_back(cubeVertices[i * 6 + 2] + z);
                                vertices.push_back(cubeVertices[i * 6 + 3]);
                                vertices.push_back(cubeVertices[i * 6 + 4]);
                                vertices.push_back(cubeVertices[i * 6 + 5]);
                            }
                        }
                    }
                }
            }
        }
        m_count = vertices.size();

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        int width, height;
        unsigned char* data = stbi_load("res/textures/grass.png", &width, &height, nullptr, 4);

        unsigned int tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glActiveTexture(GL_TEXTURE0);
        // TERRAIN START
    }

    void render(unsigned int shader)
    {
        glUseProgram(shader);
        glm::mat4 model(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, m_count);
    }

private:
    bool m_blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    unsigned int m_count;
};
