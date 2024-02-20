#version 400 core

in vec2 texCoord; // Текстурные координаты из вершинного шейдера

out vec4 FragColor;

uniform sampler2D sampler;

void main()
{
    // Простой выбор цвета из текстуры
    FragColor = texture(sampler, texCoord);
}