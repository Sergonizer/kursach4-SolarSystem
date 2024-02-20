#ifndef MOON_H
#define MOON_H

#include "object.h"

class Moon : public Object 
{
public:
    Moon(float radius, float distance, float orbitTime, float rotationTime, float axisAngle, float planetAngle, const char* texturePath);

    void Update(float elapsedTime, const glm::vec3& newParentPosition);
    void Draw(Shader shader) override;
    void DrawOrbit(Shader orbitShader);

private:
    float radius;
    float distance;
    float orbitTime;
    float rotationTime;
    float axisAngle;
    glm::vec3 position;
    glm::vec3 parentPosition;
    glm::vec3 planetAxis;
    glm::mat4 modelMatrix;
    VaoVbo orbitVaoVbo;

    void GenerateMoon();
    void GenerateOrbit();
};

#endif