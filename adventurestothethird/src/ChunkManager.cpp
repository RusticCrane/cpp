#include "ChunkManager.h"

#include "Chunk.h"

ChunkManager::ChunkManager(Camera& camera, const sf::Window& window) : m_camera(camera), m_window(window)
{
    m_updatingChunksThread = std::thread(&ChunkManager::UpdatingChunksThread, this);
}

void ChunkManager::CreateChunk(int x, int y, int z)
{
    Chunk* chunk = new Chunk();
	chunk->SetPosition(x, y, z);
	chunk->Setup();
    chunk->CreateMesh();
    m_chunks.insert({ glm::ivec3(x, y, z), chunk });
}

void ChunkManager::GenerateChunks() { }

void ChunkManager::InitialiseChunkCreation()
{
    CreateChunk(0, 0, 0);
}

void ChunkManager::Update(double dt) { }

void ChunkManager::Render(Shader& shader)
{
    int vertices = 0;
    for (auto& element : m_chunks)
    {
        auto chunk = element.second;
        chunk->Render(shader);
        vertices += chunk->GetCount();
    }
    std::cout << vertices / 3 << " triangles drawn" << std::endl;
}

void ChunkManager::UpdatingChunksThread()
{
    while (true)
    {
        constexpr int MAX_CHUNKS_LOADED = 1;
        std::vector<Chunk*> chunks;
        chunks.reserve(m_chunks.size());
        for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
        {
            chunks.push_back(it->second);
        }

        std::vector<glm::ivec3> chunksToAdd;
        std::vector<Chunk*> chunksToUpdate;
        std::vector<Chunk*> chunksToRemove;
        for (int i = 0; i < chunks.size(); ++i)
        {
            Chunk* chunk = chunks[i];
            glm::vec3 chunkGridPosition = chunk->GetGridPos();
			glm::vec3 chunkWorldPosition = chunkGridPosition * static_cast<float>(Chunk::CHUNK_SIZE);
            chunkWorldPosition += Chunk::CHUNK_SIZE / 2;
            glm::vec3 distanceVector = chunkWorldPosition - m_camera.position();
            float distance = distanceVector.length();
            if (distance > RENDER_DISTANCE)
            {
                chunksToRemove.push_back(chunk);
            }
            else
            {
				chunksToUpdate.push_back(chunk);
                if (chunk->GetChunkXPlus() == nullptr)
                {
					glm::vec3 chunkPos = chunkWorldPosition;
					chunkPos.x += Chunk::CHUNK_SIZE;
					glm::vec3 dVector = chunkPos - m_camera.position();
					float d = dVector.length();
                    if (d <= RENDER_DISTANCE)
                    {
						chunksToAdd.push_back(glm::ivec3(chunkGridPosition.x + 1, chunkGridPosition.y, chunkGridPosition.z));
                    }
                }
            }

            
            // Calculate distance from chunk to camera
            // If chunk is close enough, add it to the "update list"
            // Check if the adjacent chunks are there, else, add them to the "add list"
            // Else add it to the "remove list"
        }
		for (glm::ivec3 pos : chunksToAdd)
		{
			m_window.setActive(true);
			CreateChunk(pos.x, pos.y, pos.z);
		}
        // Loop through existing chu
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
