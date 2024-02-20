#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

struct tex_info
{
    tex_info()
        : m_RendererID(0)
        , m_AverageColor({ 1, 1, 1, 1 })
        , m_Width(0), m_Height(0), m_BPP(3)
    {}

    GLuint m_RendererID;
    glm::vec4 m_AverageColor;
    int m_Width, m_Height, m_BPP;
};

class Texture 
{
public:
    Texture() = default;
    Texture(const std::string& path);
    void Bind() const;
    GLuint GetRenderID() const;
    glm::vec4 GetAverageColor() const;
    ~Texture();

private:
    tex_info info;
    std::string m_FilePath;
};

#endif