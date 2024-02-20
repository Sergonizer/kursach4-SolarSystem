#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "common.h"
#include "shader.h"

class Crosshair
{
public:
    Crosshair(GLFWwindow* window);

    void Draw(const glm::vec4& crosshairColorValue);
    void Update(GLFWwindow* window);

private:
    Shader shader;
    GLuint VAO, VBO;
    float crosshairSize = 0.05f;
};

#endif