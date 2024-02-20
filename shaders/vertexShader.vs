#version 400 core
layout(location = 0) in vec3 inPosition;   // ������� �������
layout(location = 1) in vec3 inNormal;     // ������� �������
layout(location = 2) in vec2 inTexCoord;   // ������� ������� ��������

out vec3 fragPos;      // �������� ������� �� ����������� ������
out vec3 fragNormal;   // �������� ������� �� ����������� ������
out vec2 texCoord;     // �������� ������� �������� �� ����������� ������

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // ������ ������� � ������� �����������
    vec4 worldPos = model * vec4(inPosition, 1.0);
    
    // ������ ������� � �������� �����������
    vec4 viewPos = view * worldPos;
    
    // ������ ������� � ������� �����������
    vec3 worldNormal = normalize(mat3(view * model) * inNormal);
    
    // ��������� ��������
    fragPos = viewPos.xyz;
    fragNormal = worldNormal;
    texCoord = inTexCoord;
    
    // ������ ��������� ������� � �������� �����������
    gl_Position = projection * viewPos;
}