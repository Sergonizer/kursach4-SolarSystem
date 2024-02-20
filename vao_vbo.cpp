#include "vao_vbo.h"

VaoVbo::VaoVbo(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices, const std::vector<GLfloat>& normals, const std::vector<GLfloat>& texCoords)
    : vertexCount(static_cast<GLsizei>(indices.size())) 
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    Bind();

    size_t totalSize = (vertices.size() + normals.size() + texCoords.size()) * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());

    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), normals.size() * sizeof(GLfloat), normals.data());

    glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size()) * sizeof(GLfloat), texCoords.size() * sizeof(GLfloat), texCoords.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertices.size() * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)((vertices.size() + normals.size()) * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    Unbind();
}

VaoVbo::VaoVbo(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices, const std::vector<GLfloat>& texCoords)
    : vertexCount(static_cast<GLsizei>(indices.size())) 
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    Bind();

    size_t totalSize = (vertices.size() + texCoords.size()) * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());

    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), texCoords.size() * sizeof(GLfloat), texCoords.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertices.size() * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    Unbind();
}

VaoVbo::VaoVbo(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
    : vertexCount(static_cast<GLsizei>(indices.size())) 
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    Bind();

    size_t totalSize = (vertices.size()) * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    Unbind();
}

void VaoVbo::Bind() 
{
    glBindVertexArray(VAO);
}

void VaoVbo::Unbind() 
{
    glBindVertexArray(0);
}

void VaoVbo::Draw() 
{
    Bind();
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
    Unbind();
}