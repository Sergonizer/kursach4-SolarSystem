#version 400 core

out vec4 FragColor;

uniform vec4 averageColor;  // ������� ����������� ������� ���� ��������

void main()
{
    FragColor = averageColor;
}