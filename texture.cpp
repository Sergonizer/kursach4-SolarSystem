#include "texture.h"
#include <iostream>

// Использование stb_image
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <vector>
#include <map>

static std::map<const std::string, tex_info> textures;

Texture::Texture(const std::string& path)
    : info(), m_FilePath(path) 
{
    auto tex = textures.find(path);
    if (tex != textures.end())
    {
        info = tex->second;
        return;
    }

    unsigned char* m_LocalBuffer(nullptr);

    glGenTextures(1, &info.m_RendererID);
    glBindTexture(GL_TEXTURE_2D, info.m_RendererID);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Загрузка текстуры
    m_LocalBuffer = stbi_load(path.c_str(), &info.m_Width, &info.m_Height, &info.m_BPP, 0);

    if (!m_LocalBuffer) 
    {
        std::cerr << "Ошибка при загрузке текстуры: " << path << std::endl;
        return;
    }

    GLenum format = GL_RGBA; // Стандартным ставится формат RGBA
    if (info.m_BPP == 3)
        format = GL_RGB;
    else if (info.m_BPP == 1)
        format = GL_RED;

    glTexImage2D(GL_TEXTURE_2D, 0, format, info.m_Width, info.m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Понижение разрешения текстуры для расчета среднего цвета
    int downscaleFactor = 8;
    int downscaleWidth = info.m_Width / downscaleFactor;
    int downscaleHeight = info.m_Height / downscaleFactor;

    std::vector<unsigned char> downscaleData(downscaleWidth * downscaleHeight * info.m_BPP);

    for (int y = 0; y < downscaleHeight; y++) 
    {
        for (int x = 0; x < downscaleWidth; x++) 
        {
            int srcX = x * downscaleFactor;
            int srcY = y * downscaleFactor;
            int dstOffset = (y * downscaleWidth + x) * info.m_BPP;

            for (int i = 0; i < info.m_BPP; i++) 
            {
                unsigned int total = 0;

                for (int offsetY = 0; offsetY < downscaleFactor; offsetY++) 
                {
                    for (int offsetX = 0; offsetX < downscaleFactor; offsetX++) 
                    {
                        int srcOffset = ((srcY + offsetY) * info.m_Width + (srcX + offsetX)) * info.m_BPP + i;
                        total += m_LocalBuffer[srcOffset];
                    }
                }

                downscaleData[dstOffset + i] = total / (downscaleFactor * downscaleFactor);
            }
        }
    }

    // Расчет среднего цвета
    unsigned int totalR = 0, totalG = 0, totalB = 0, totalA = 0;
    unsigned int totalPixels = downscaleWidth * downscaleHeight;

    for (unsigned int i = 0; i < totalPixels; i++) 
    {
        unsigned int offset = i * info.m_BPP;
        totalR += downscaleData[offset];
        totalG += downscaleData[offset + 1];
        totalB += downscaleData[offset + 2];
        if (info.m_BPP > 3)
            totalA += downscaleData[offset + 3];
        else
            totalA += 255;
    }

    unsigned char avgR = totalR / totalPixels;
    unsigned char avgG = totalG / totalPixels;
    unsigned char avgB = totalB / totalPixels;
    unsigned char avgA = totalA / totalPixels;

    info.m_AverageColor = glm::vec4(avgR / 255.0f, avgG / 255.0f, avgB / 255.0f, avgA / 255.0f);

    textures.insert({ path, info });
}

Texture::~Texture() 
{
	//glDeleteTextures(1, &info.m_RendererID); // Вызывает проблемы при создании (emplace_back вызывает деструктор)
}

void Texture::Bind() const 
{
	glBindTexture(GL_TEXTURE_2D, info.m_RendererID);
}

GLuint Texture::GetRenderID() const 
{
    return info.m_RendererID;
}

glm::vec4 Texture::GetAverageColor() const 
{
    return info.m_AverageColor;
}