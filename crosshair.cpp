#include "crosshair.h"

Crosshair::Crosshair(GLFWwindow* window)
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Расчет соотношение сторон
    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    // Расчет ширины прицела исходя из соотношения сторон
    float adjustedSize = crosshairSize / aspectRatio;

    std::vector<GLfloat> vertices = {
        -adjustedSize, 0.0f, 0.0f,
        adjustedSize, 0.0f, 0.0f,
        0.0f, -crosshairSize, 0.0f,
        0.0f, crosshairSize, 0.0f
    };

    std::vector<GLuint> indices = {
    0, 1, // Горизонтальная линия
    2, 3  // Вертикальная линия
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    shader = Shader("shaders/chVertexShader.vs", "shaders/colorFragmentShader.fs");
}

void Crosshair::Update(GLFWwindow* window)
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    float adjustedSize = crosshairSize / aspectRatio;

    std::vector<GLfloat> vertices = {
        -adjustedSize, 0.0f, 0.0f,
        adjustedSize, 0.0f, 0.0f,
        0.0f, -crosshairSize, 0.0f,
        0.0f, crosshairSize, 0.0f
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Crosshair::Draw(const glm::vec4& crosshairColorValue)
{
    shader.Use();
    shader.SetVec4("averageColor", crosshairColorValue);

    glBindVertexArray(VAO);

    glLineWidth(2);
    glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
