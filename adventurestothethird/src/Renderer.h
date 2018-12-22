#pragma once

#include "pch.h"

struct OpenGLPositionNormalVertex
{
    float x, y, z;
    float nx, ny, nz;
};

class Renderer
{
public:
    Renderer();
    ~Renderer();
};
