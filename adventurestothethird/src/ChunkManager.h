#pragma once

#include <thread>
#include <unordered_map>

#include <SFML/Window.hpp>

#include "Shader.h"

#include "Camera.h"
#include <iostream>
#include <mutex>
//#include "Player.h"

#define RENDER_DISTANCE 7

class Chunk;

namespace std
{
    template<>
    struct hash<glm::ivec3>
    {
        std::size_t operator()(const glm::ivec3& k) const
        {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return ((hash<int>()(k.x)
                        ^ (hash<int>()(k.y) << 1)) >> 1)
                    ^ (hash<int>()(k.z) << 1);
        }
    };
}

class ChunkManager
{
public:
    /* Public methods */
    ChunkManager(/*const Player& player*/ Camera& camera, const sf::Window& window);
    /*
     * Generates a chunk for the given grid coordinate (x, y, z)
     */
	void CreateChunk(int x, int y, int z);
    void Update(double dt);
    void Render(Shader& shader);

	void UpdatingChunksThread();

	void GenerateChunks();
	void InitialiseChunkCreation();

protected:
    /* Protected methods */

private:
    /* Private methods */

public:
    /* Private members*/

protected:
    /* Protected members */

private:
    /* Private methods */
    //const Player& m_player;
    Camera& m_camera;
	const sf::Window& m_window;
    std::unordered_map<glm::ivec3, Chunk*> m_chunks;
	std::thread m_updatingChunksThread;
	std::mutex m_contextMutex;
};
