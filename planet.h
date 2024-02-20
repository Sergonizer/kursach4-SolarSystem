#ifndef PLANET_H
#define PLANET_H

#include "object.h"
#include "moon.h"
#include "ring.h"

class Planet : public Object
{
public:
    Planet() = default;
    Planet(float radius, float distance, float orbitTime, float rotationTime, float axisAngle, const char* texturePath, float ringRad = 0, float ringDist = 0, const char* ringTexturePath = nullptr);

    void Draw(Shader shader) override;
    void DrawOrbit(Shader orbitShader);
    void DrawRings(Shader ringShader);
    void Update(float elapsedTime);
    glm::vec3 GetPosition();
    float GetRadius();
    Texture GetTexture();
    void AddMoon();
    glm::mat4 GetModelMatrix() const;

private:
    float radius;
    float distance;
    float orbitTime;
    float rotationTime;
    float axisAngle;
    glm::vec3 position;
    glm::mat4 modelMatrix;
    VaoVbo orbitVaoVbo;
    std::optional<Ring> ring;
    std::vector<Moon> moons;

    void GeneratePlanet();
    void GenerateOrbit();
};

#endif