#pragma once

#include "Chunk.h"

class ChunkGenerator
{
public:
    template<unsigned int Size>
    Chunk<Size> GenerateChunk(int x, int y, int z)
    {
        FastNoise noise(24);
        noise.SetNoiseType(FastNoise::Perlin);
        noise.SetFrequency(0.05f);

        std::array<std::array<std::array<bool, Size>, Size>, Size> blocks;

        for (int i = 0; i < Size; ++i)
        {
            for (int j = 0; j < Size; ++j)
            {
                for (int k = 0; k < Size; ++k)
                {
                    blocks[i][j][k] = noise.GetNoise(i + x * Size, j + y * Size,
                                                     k + z * Size) > 0.6 - ((static_cast<double>(Size) - j) / Size * 1.2
                    );
                }
            }
        }
        Chunk<Size> chunk(blocks);
    }
};
