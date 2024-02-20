#ifndef RING_H
#define RING_H

#include "object.h"

class Ring : public Object
{
public:
    Ring() = default;
    Ring(float radius, float width, const char* texturePath);

    void Draw(Shader shader, const glm::mat4& planetModelMatrix);
    void Draw(Shader shader) override { return; }

private:
    float width;
    float radius;
    float rotationTime;

    void GenerateRing();
};

#endif