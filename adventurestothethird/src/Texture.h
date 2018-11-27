#pragma once

#include <string>

class Texture
{
public:
    Texture(const std::string& path);
    void Bind() const;
    void Unbind() const;
private:
    unsigned int m_id;
};


