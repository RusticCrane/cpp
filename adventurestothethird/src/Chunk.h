#pragma once

#include <chrono>
#include "FastNoise/FastNoise.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexArray.h"

#include <array>

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
    void GenerateChunk(int xOff, int yOff, int zOff)
    {
        FastNoise noise(24);
        noise.SetNoiseType(FastNoise::Perlin);
        noise.SetFrequency(0.05f);

        for (int x = 0; x < CHUNK_SIZE; ++x)
        {
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                for (int z = 0; z < CHUNK_SIZE; ++z)
                {
                    double k = static_cast<double>(CHUNK_SIZE) - y;
                    k /= CHUNK_SIZE;
                    k *= 1.2;
                    double v = 0.6 - k;
                    m_blocks[x][y][z] = noise.GetNoise(x + xOff * CHUNK_SIZE, y + yOff * CHUNK_SIZE,
                                                       z + zOff * CHUNK_SIZE) > v;
                }
            }
        }
    }

    std::pair<VertexArray, int> CreateChunkMesh()
    {
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

        VertexArray va;

        VertexBuffer vb(vertices.data(), vertices.size() * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        va.AddBuffer(vb, layout);
        return std::pair<VertexArray, int>(va, m_count);
    }

    Chunk()
    {
        for (int x = 0; x < 5; ++x)
        {
            for (int z = 0; z < 5; ++z)
            {
                GenerateChunk(x, 0, z);
                m_vas.at(x).at(z) = CreateChunkMesh();
            }
        }
    }

    void render(unsigned int shader)
    {
        glUseProgram(shader);
        int loc = glGetUniformLocation(shader, "model");
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                m_vas.at(i).at(j).first.Bind();
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(CHUNK_SIZE * i, 0.0f, CHUNK_SIZE * j));

                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, m_vas.at(i).at(j).second);
            }
        }
    }

private:
    bool m_blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    unsigned int m_count;
    std::array<std::array<std::pair<VertexArray, int>, 5>, 5> m_vas;
};
