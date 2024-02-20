#version 400 core

out vec4 FragColor;

uniform vec4 averageColor;  // Заранее вычисленный средний цвет текстуры

void main()
{
    FragColor = averageColor;
}