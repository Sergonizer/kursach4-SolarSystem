#include "planet.h"

#define random static_cast<float>(rand()*1./RAND_MAX)

Planet::Planet(float radius, float distance, float orbitTime, float rotationTime, float axisAngle, const char* texturePath, float ringRad, float ringWidth, const char* ringTexturePath)
    : radius(radius), distance(distance), orbitTime(orbitTime), rotationTime(rotationTime), axisAngle(axisAngle), Object(texturePath), position(0, 0, 0)
{
    if (ringRad != 0 && ringWidth != 0 && ringTexturePath != nullptr)
        ring = Ring(ringRad, ringWidth, ringTexturePath);
    GeneratePlanet();
    GenerateOrbit();
}

void Planet::Draw(Shader shader)
{
    shader.Use();
    glActiveTexture(GL_TEXTURE0);
    shader.SetMat4("model", modelMatrix);
    texture.Bind();
    vaoVbo.Draw();

    // Отрисовка лун
    for (auto& moon : moons)
        moon.Draw(shader);
}

void Planet::DrawOrbit(Shader orbitShader)
{
    orbitShader.Use();
    glActiveTexture(GL_TEXTURE0);
    glm::mat4 orbitModel = glm::mat4(1.0f);
    orbitShader.SetMat4("model", orbitModel);
    orbitShader.SetVec4("averageColor", texture.GetAverageColor());
    texture.Bind();
    orbitVaoVbo.Draw();

    // Отрисовка орбит лун
    for (auto& moon : moons)
        moon.DrawOrbit(orbitShader);
}

void Planet::DrawRings(Shader ringShader)
{
    if (ring)
        ring->Draw(ringShader, modelMatrix);
}

void Planet::Update(float elapsedTime)
{
    modelMatrix = glm::mat4(1.0f);

    // Постоянный угол в плоскости ZY
    float radAngle = glm::radians(axisAngle);
    modelMatrix = glm::rotate(modelMatrix, radAngle, glm::vec3(1.0f, 0.0f, 0.0f));

    // Вращение вокруг своей оси
    float degRotAngle = (elapsedTime / rotationTime) * 360.0f;
    while (degRotAngle >= 360.0f) degRotAngle -= 360.0f;
    float selfRotationAngle = glm::radians(degRotAngle);

    modelMatrix = glm::rotate(modelMatrix, selfRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

    // Вращение вокруг солнца
    float degOrbAngle = (elapsedTime / orbitTime) * 360.0f;
    while (degOrbAngle >= 360.0f) degOrbAngle -= 360.0f;
    float orbitAngle = -glm::radians(degOrbAngle);

    // Обновление позиции исходя из вычисленного значения
    position.x = distance * glm::cos(orbitAngle);
    position.z = distance * glm::sin(orbitAngle);

    // Перенос на позицию в мировых координатах
    modelMatrix = glm::translate(glm::mat4(1.0f), position) * modelMatrix;

    // Обновление лун
    for (auto& moon : moons)
        moon.Update(elapsedTime, position);
}

glm::vec3 Planet::GetPosition()
{
    return position;
}

float Planet::GetRadius()
{
    return radius;
}

Texture Planet::GetTexture()
{
    return texture;
}

void Planet::AddMoon()
{
    // Используются случайные значения 
    float randomRadius = (0.1f + random * 0.2f) * radius;
    float randomDistance = radius + (0.5f + random * 4.5f) * radius;
    float randomOrbitTime = 15.0f * rotationTime / (0.5f + random * 30.f);
    float randomRotationTime = 15.0f * rotationTime / (0.5f + random * 30.f);
    float randomAxisAngle = random * 360.0f;

    moons.emplace_back(randomRadius, randomDistance, randomOrbitTime, randomRotationTime, randomAxisAngle, axisAngle, "images/moon.tga");
}

glm::mat4 Planet::GetModelMatrix() const
{
    return modelMatrix;
}

void Planet::GeneratePlanet()
{
    const int latDivisions = 15;  // Количество широт
    const int longDivisions = 100; // Количество долгот

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

            // Вычисление нормали для каждой точки
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
        }
    }

    // Индексы для создания треугольников
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

void Planet::GenerateOrbit()
{
    const int numTheta = 3600; // Количество точек на орбите
    const int numPhi = 10;   // Количество точек для самой линии орбиты

    std::vector<GLfloat> orbitVertices;
    std::vector<GLuint> orbitIndices;

    const float tubeRadius = radius / 32; // Радиус линии орбиты
    const float ringRadius = distance;    // Радиус орбиты

    for (int i = 0; i <= numTheta; ++i)
    {
        float theta = glm::radians(static_cast<float>(i * 360.0 / numTheta));
        for (int j = 0; j <= numPhi; ++j)
        {
            float phi = glm::radians(static_cast<float>(j * 360.0 / numPhi));

            float x = (ringRadius + tubeRadius * glm::cos(phi)) * glm::cos(theta);
            float y = tubeRadius * glm::sin(phi);
            float z = (ringRadius + tubeRadius * glm::cos(phi)) * glm::sin(theta);

            orbitVertices.push_back(x);
            orbitVertices.push_back(y);
            orbitVertices.push_back(z);
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