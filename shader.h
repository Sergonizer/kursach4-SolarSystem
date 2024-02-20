#ifndef SHADER_H
#define SHADER_H

#include "common.h"
#include <string>

class Shader
{
public:
    Shader() = default;
    Shader(const char* vertexPath, const char* fragmentPath);
    GLuint GetID();
    void Use();
    void SetMat4(const std::string& name, const glm::mat4& matrix);
    void SetVec3(const std::string& name, const glm::vec3& value);
    void SetVec4(const std::string& name, const glm::vec4& value);

private:
    GLuint programID;

    void CheckCompileErrors(GLuint shader, std::string type);
};

#endif