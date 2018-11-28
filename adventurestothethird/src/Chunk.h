#pragma once

#include <chrono>
#include "FastNoise/FastNoise.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexArray.h"

#include <array>

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

template<unsigned int Size = 16>
class Chunk
{
public:
    Chunk(std::array<std::array<std::array<bool, Size>, Size>, Size> blocks) : m_blocks(blocks) { }

    std::pair<VertexArray, int> CreateChunkMesh()
    {
        std::vector<float> vertices;

        for (int x = 0; x < Size; ++x)
        {
            for (int y = 0; y < Size; ++y)
            {
                for (int z = 0; z < Size; ++z)
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
                        if (z == Size - 1 || (z < Size - 1 && !m_blocks[x][y][z + 1]))
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
                        if (x == Size - 1 || (x < Size - 1 && !m_blocks[x + 1][y][z]))
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
                        if (y == Size - 1 || (y < Size - 1 && !m_blocks[x][y + 1][z]))
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

        VertexBuffer vb(vertices.data(), vertices.size() * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        m_va.AddBuffer(vb, layout);
    }

    void Render(unsigned int shader, int x, int y, int z)
    {
        glUseProgram(shader);
        m_va.Bind();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(Size * x, Size * y, Size * z));
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, m_count);
    }

private:
    std::array<std::array<std::array<bool, Size>, Size>, Size> m_blocks;
    unsigned int m_count;
    VertexArray m_va;
};
