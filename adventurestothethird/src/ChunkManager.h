#pragma once

#include <map>

#include "Chunk.h"

//#include "Player.h"

class ChunkManager
{
public:
    /* Public methods */
    ChunkManager(/*const Player& player*/);

    glm::vec3 getPosition() const
    {
        return glm::vec3(0.0f, 48.0f, 0.0f);
    }

    void update(double dt) { }

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
    std::map<glm::vec3, Chunk<16>> m_chunks;
};
