#include "ring.h"

Ring::Ring(float radius, float width, const char* texturePath)
    : radius(radius), width(width), Object(texturePath)
{
    GenerateRing();
}

void Ring::Draw(Shader shader, const glm::mat4& planetModelMatrix)
{
    shader.Use();
    glActiveTexture(GL_TEXTURE0);

    shader.SetMat4("model", planetModelMatrix);
    texture.Bind();
    vaoVbo.Draw();
}

void Ring::GenerateRing()
{
    const int ringDivisions = 100;

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;

    float innerRadius = radius;
    float outerRadius = radius + width;

    for (int lon = 0; lon <= ringDivisions; ++lon)
    {
        float phi = lon * 2 * 3.14159f / ringDivisions;
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);

        // Внешнее кольцо
        vertices.push_back(outerRadius * cosPhi);
        vertices.push_back(0.0f);
        vertices.push_back(outerRadius * sinPhi);

        texCoords.push_back(1.f);
        texCoords.push_back(0.f);

        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);

        // Внутреннее кольцо
        vertices.push_back(innerRadius * cosPhi);
        vertices.push_back(0.0f);
        vertices.push_back(innerRadius * sinPhi);

        texCoords.push_back(0.f);
        texCoords.push_back(1.f);

        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);
    }

    for (int lon = 0; lon < ringDivisions; ++lon)
    {
        int first = 2 * lon;
        int second = first + 1;
        int third = (first + 2) % (2 * ringDivisions);
        int fourth = (second + 2) % (2 * ringDivisions);

        indices.push_back(first);
        indices.push_back(second);
        indices.push_back(third);

        indices.push_back(second);
        indices.push_back(fourth);
        indices.push_back(third);
    }

    vaoVbo = VaoVbo(vertices, indices, normals, texCoords);
}