#include "skybox.h"

SkyBox::SkyBox(const char* texturePath)
    : Object(texturePath)
{
    const int latDivisions = 30;
    const int longDivisions = 30;

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> texCoords;

    for (int lat = 0; lat <= latDivisions; ++lat) 
    {
        float theta = lat * 3.14159f / latDivisions;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= longDivisions; ++lon) 
        {
            float phi = lon * 2 * 3.14159f / longDivisions;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            float u = static_cast<float>(lon) / longDivisions;
            float v = static_cast<float>(lat) / latDivisions;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            texCoords.push_back(u);
            texCoords.push_back(v);
        }
    }

    for (int lat = 0; lat < latDivisions; ++lat) 
    {
        for (int lon = 0; lon < longDivisions; ++lon) 
        {
            int first = lat * (longDivisions + 1) + lon;
            int second = first + 1;
            int third = (lat + 1) * (longDivisions + 1) + lon;
            int fourth = third + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(third);

            indices.push_back(second);
            indices.push_back(fourth);
            indices.push_back(third);
        }
    }

    vaoVbo = VaoVbo(vertices, indices, texCoords);
}

void SkyBox::Draw(Shader shader) 
{
    glDepthFunc(GL_LEQUAL);
    glDepthRange(1, 1);
    shader.Use();

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    vaoVbo.Draw();
    glDepthRange(0, 0);
}