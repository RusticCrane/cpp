#include "Chunk.h"

#include "FastNoise/FastNoise.h"

#include "ChunkManager.h"

Chunk::Chunk(ChunkManager& chunkManager) : m_chunkManager(chunkManager) {}

void Chunk::Setup()
{
    if (m_setup)
    {
        return;
    }

    FastNoise noise(24);
    noise.SetNoiseType(FastNoise::Perlin);
    noise.SetFrequency(0.05f);

    int max = 25;
    int min = 5;

    auto lm = [](double mi, double ma, double va) -> double
    {
        return (va - mi) / (ma - mi) * 2.0;
    };

    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                SetActive(x, y, z,
                          noise.GetNoise(x + m_position.x * CHUNK_SIZE, z + m_position.z * CHUNK_SIZE) > -1.0 +
                          lm(10, 25, y + m_position.y * CHUNK_SIZE));
            }
        }
    }

    m_setup = true;
}

bool Chunk::IsSetup() const
{
    return m_setup;
}

void Chunk::CreateMesh()
{
    std::vector<float> vertices;

    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_SIZE; ++y)
        {
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                if (GetActive(x, y, z))
                {
                    // BACK
                    if (z == 0 || (z > 0 && !GetActive(x, y, z - 1)))
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
                    if (z == CHUNK_SIZE - 1 || (z < CHUNK_SIZE - 1 && !GetActive(x, y, z + 1)))
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
                    if (x == 0 || (x > 0 && !GetActive(x - 1, y, z)))
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
                    if (x == CHUNK_SIZE - 1 || (x < CHUNK_SIZE - 1 && !GetActive(x + 1, y, z)))
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
                    if (y == 0 || (y > 0 && !GetActive(x, y - 1, z)))
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
                    if (y == CHUNK_SIZE - 1 || (y < CHUNK_SIZE - 1 && !GetActive(x, y + 1, z)))
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

	m_va = std::make_unique<VertexArray>();
    m_va->AddBuffer(vb, layout);

	m_rebuild = false;
}

void Chunk::Render(Shader& shader)
{
    if (m_va)
    {
		shader.Use();
		m_va->Bind();
		glm::mat4 model = glm::translate(glm::mat4(1.0f),
			glm::vec3(CHUNK_SIZE * m_position.x, CHUNK_SIZE * m_position.y,
				CHUNK_SIZE * m_position.z));
		shader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, m_count);
    }
    
}

int Chunk::GetCount() const
{
    return m_count;
}

bool Chunk::GetActive(int x, int y, int z)
{
    return m_blocks[x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z];
}

void Chunk::SetActive(int x, int y, int z, bool active)
{
    m_blocks[x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z] = active;
}

void Chunk::SetPosition(int x, int y, int z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

bool Chunk::NeedsRebuild()
{
	return m_rebuild;
}

void Chunk::SetNeedsRebuild(bool rebuild)
{
	m_rebuild = rebuild;
}
