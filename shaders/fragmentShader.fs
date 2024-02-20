#version 400 core

in vec3 fragPos; // ��������� ��������� �� ���������� �������
in vec3 fragNormal; // ������� �� ���������� �������
in vec2 texCoord; // ���������� ���������� �� ���������� �������

out vec4 FragColor;

uniform sampler2D planetTexture;
uniform vec3 lightPos; // ��������� ��������� ����� � ������� �����������
uniform mat4 view; // ������� ������

void main()
{
    // ����� ����� �� �������� ����� ����������
    vec4 textureColor = texture(planetTexture, texCoord);
    
    // �� ������ ������ �������������� ������������ ������� �������
    vec3 normalizedNormal = normalize(fragNormal);
    
    // ��������� ������ �� ��������� ��������� � ��������� �����
    // ������������� ��������� ����� �� ������ ������� ����
    vec4 transformedLightPos = view * vec4(lightPos, 1.0);
    vec3 lightDir = normalize(transformedLightPos.xyz - fragPos);
    float distance = length(fragPos - transformedLightPos.xyz); // ��������� ����������
    
    float distanceMod = (1 / (1.0 + 0.05 * distance));
    
    // ��������� ����������� ���������� ���������
    float diffuseFactor = distanceMod * max(dot(normalizedNormal, lightDir), 0.0);
    
    // ��������� ��������� ��������� � ����� ��������
    vec3 diffuseColor = vec3(0.2, 0.2, 0.2) * diffuseFactor; // ������ ����� ���� ��� ������ ���������
    
    // ������� ���� �������� � ������ ���������
    vec3 finalColor = distanceMod * textureColor.rgb;
    
    // ����������� ��������� ��� ������ �������
    float dotProduct = max(dot(normalizedNormal, lightDir), 0.05);
    finalColor *= dotProduct;
    finalColor += diffuseColor;
    
    // ������������ �����-����� �� ��������
    FragColor = vec4(finalColor, textureColor.a);
}