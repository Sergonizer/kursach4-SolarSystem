#include "moon.h"

Moon::Moon(float radius, float distance, float orbitTime, float rotationTime, float axisAngle, float planetAngle, const char* texturePath)
    : radius(radius), distance(distance), orbitTime(orbitTime), rotationTime(rotationTime), axisAngle(axisAngle), Object(texturePath), position(0, 0, 0),
    planetAxis(glm::rotate(glm::mat4(1.0f), glm::radians(planetAngle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f))
{
    GenerateMoon();
    GenerateOrbit();
}

void Moon::Update(float elapsedTime, const glm::vec3& newParentPosition)
{
    parentPosition = newParentPosition;
    modelMatrix = glm::mat4(1.0f);

    // Постоянный угол в плоскости ZY
    float radAngle = glm::radians(axisAngle);
    modelMatrix = glm::rotate(modelMatrix, radAngle, glm::vec3(1.0f, 0.0f, 0.0f));

    // Вращение вокруг своей оси
    float degRotAngle = (elapsedTime / rotationTime) * 360.0f;
    while (degRotAngle >= 360.0f) degRotAngle -= 360.0f;
    float selfRotationAngle = glm::radians(degRotAngle);
    modelMatrix = glm::rotate(modelMatrix, selfRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Вращение вокруг оси планеты
    float degOrbAngle = (elapsedTime / orbitTime) * 360.0f;
    while (degOrbAngle >= 360.0f) degOrbAngle -= 360.0f;
    float orbitAngle = glm::radians(degOrbAngle);

    glm::mat4 orbitRotationMatrix = glm::rotate(glm::mat4(1.0f), orbitAngle, planetAxis);

    glm::vec3 perpAxis;
    if (planetAxis != glm::vec3(1.0f, 0.0f, 0.0f))
        perpAxis = glm::cross(planetAxis, glm::vec3(1.0f, 0.0f, 0.0f));
    else
        perpAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 posVec = glm::normalize(glm::vec3(orbitRotationMatrix * glm::vec4(perpAxis, 1.0f)));

    // Обновление позиции исходя из вычисленного значения
    position = posVec * distance + parentPosition;

    // Перенос на позицию в мировых координатах
    modelMatrix = glm::translate(glm::mat4(1.0f), position) * modelMatrix;
}

void Moon::Draw(Shader shader)
{
    shader.Use();
    glActiveTexture(GL_TEXTURE0);
    shader.SetMat4("model", modelMatrix);
    texture.Bind();
    vaoVbo.Draw();
}

void Moon::DrawOrbit(Shader orbitShader)
{
    orbitShader.Use();
    glActiveTexture(GL_TEXTURE0);
    glm::mat4 orbitModel = glm::mat4(1.0f);
    orbitModel = glm::translate(orbitModel, parentPosition);
    orbitShader.SetMat4("model", orbitModel);
    orbitShader.SetVec4("averageColor", texture.GetAverageColor());
    texture.Bind();
    orbitVaoVbo.Draw();
}

void Moon::GenerateMoon()
{
    const int latDivisions = 15;
    const int longDivisions = 100;

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> normals;
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

            vertices.push_back(radius * x);
            vertices.push_back(radius * y);
            vertices.push_back(radius * z);

            texCoords.push_back(u);
            texCoords.push_back(v);

            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
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

    vaoVbo = VaoVbo(vertices, indices, normals, texCoords);
}

void Moon::GenerateOrbit()
{
    const int numTheta = 3600;
    const int numPhi = 10;

    std::vector<GLfloat> orbitVertices;
    std::vector<GLuint> orbitIndices;

    const float tubeRadius = radius / 32;
    const float ringRadius = distance;

    for (int i = 0; i <= numTheta; ++i)
    {
        float theta = glm::radians(static_cast<float>(i * 360.0 / numTheta));

        glm::mat4 orbitRotationMatrix = glm::rotate(glm::mat4(1.0f), theta, planetAxis);

        for (int j = 0; j <= numPhi; ++j)
        {
            float phi = glm::radians(static_cast<float>(j * 360.0 / numPhi));

            float x = (ringRadius + tubeRadius * glm::cos(phi));
            float y = tubeRadius * glm::sin(phi);
            float z = 0.0f;

            glm::vec4 rotatedPoint = orbitRotationMatrix * glm::vec4(x, y, z, 1.0f);

            orbitVertices.push_back(rotatedPoint.x);
            orbitVertices.push_back(rotatedPoint.y);
            orbitVertices.push_back(rotatedPoint.z);
        }
    }

    for (int i = 0; i < numTheta; ++i)
    {
        for (int j = 0; j < numPhi; ++j)
        {
            int current = i * (numPhi + 1) + j;
            int next = (i + 1) * (numPhi + 1) + j;

            orbitIndices.push_back(current);
            orbitIndices.push_back(next + 1);
            orbitIndices.push_back(next);

            orbitIndices.push_back(current);
            orbitIndices.push_back(current + 1);
            orbitIndices.push_back(next + 1);
        }
    }

    orbitVaoVbo = VaoVbo(orbitVertices, orbitIndices);
}