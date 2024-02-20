#ifndef VAO_VBO_H
#define VAO_VBO_H

#include "common.h"
#include <vector>

class VaoVbo
{
public:
    VaoVbo() = default;
    VaoVbo(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices, const std::vector<GLfloat>& normals, const std::vector<GLfloat>& texCoords);
    VaoVbo(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices, const std::vector<GLfloat>& texCoords);
    VaoVbo(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
    void Bind();
    void Unbind();
    void Draw();

private:
    GLuint VAO, VBO, EBO;
    GLsizei vertexCount;
};

#endif