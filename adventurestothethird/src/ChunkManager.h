#pragma once

#include <thread>
#include <unordered_map>

#include "Shader.h"

#include "Camera.h"
#include <iostream>
//#include "Player.h"

#define RENDER_DISTANCE 3

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
    ChunkManager(/*const Player& player*/ Camera& camera);
    glm::vec3 getPosition();
    void Update(double dt);
    void Render(Shader& shader);

	void UpdatingChunksThread();

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
    std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_chunks;
	std::thread m_updatingChunksThread;
};
