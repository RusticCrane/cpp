#include "ChunkManager.h"

#include "Chunk.h"

ChunkManager::ChunkManager(/*const Player& player*/ Camera& camera) /*: m_player(player)*/ : m_camera(camera)
{
    m_updatingChunksThread = std::thread(&ChunkManager::UpdatingChunksThread, this);
}

glm::vec3 ChunkManager::getPosition()
{
    return m_camera.position();
}

void ChunkManager::Update(double dt)
{
    glm::vec3 position = getPosition();
    glm::vec3 currentChunk(
        static_cast<int>(position.x / 16),
        static_cast<int>(position.y / 16),
        static_cast<int>(position.z / 16)
    );
    for (int x = -RENDER_DISTANCE + currentChunk.x; x < RENDER_DISTANCE + currentChunk.x; ++x)
    {
        for (int y = currentChunk.y - RENDER_DISTANCE; y < currentChunk.y + RENDER_DISTANCE; ++y)
        {
            if (y < 0)
            {
                continue;
            }
            for (int z = -RENDER_DISTANCE + currentChunk.z; z < RENDER_DISTANCE + currentChunk.z; ++z)
            {
                auto chunk = m_chunks.find(glm::vec3(x, y, z));
                if (chunk == m_chunks.end())
                {
                    std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(*this);
                    chunk.get()->SetPosition(x, y, z);
                    chunk.get()->Setup();
                    m_chunks.insert({ glm::ivec3(x, y, z), chunk });
                }
                else
                {
                    if (chunk->second.get()->IsSetup() && chunk->second.get()->NeedsRebuild())
                    {
                        chunk->second.get()->CreateMesh();
                    }
                }
            }
        }
    }
}

void ChunkManager::Render(Shader& shader)
{
    int vertices = 0;
    for (auto& element : m_chunks)
    {
        auto& position = element.first;
        auto& chunk = element.second;
        chunk.get()->Render(shader);
        vertices += chunk.get()->GetCount();
    }
    //std::cout << vertices << " drawn" << std::endl;
}

void ChunkManager::UpdatingChunksThread()
{
    while (true)
    {
        for (auto& c : m_chunks)
        {
            std::shared_ptr<Chunk> chunk = c.second;
        }
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
