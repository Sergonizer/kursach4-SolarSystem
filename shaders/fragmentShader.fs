#version 400 core

in vec3 fragPos; // Положение фрагмента из вершинного шейдера
in vec3 fragNormal; // Нормаль из вершинного шейдера
in vec2 texCoord; // Текстурные координаты из вершинного шейдера

out vec4 FragColor;

uniform sampler2D planetTexture;
uniform vec3 lightPos; // Положение источника света в мировых координатах
uniform mat4 view; // Матрица обзора

void main()
{
    // Выбор цвета из текстуры через координаты
    vec4 textureColor = texture(planetTexture, texCoord);
    
    // На всякий случай дополнительная нормализация вектора нормали
    vec3 normalizedNormal = normalize(fragNormal);
    
    // Вычислить вектор от положения фрагмента к источнику света
    // Преобразовать положение света на основе матрицы вида
    vec4 transformedLightPos = view * vec4(lightPos, 1.0);
    vec3 lightDir = normalize(transformedLightPos.xyz - fragPos);
    float distance = length(fragPos - transformedLightPos.xyz); // Вычислить расстояние
    
    float distanceMod = (1 / (1.0 + 0.05 * distance));
    
    // Вычислить коэффициент диффузного освещения
    float diffuseFactor = distanceMod * max(dot(normalizedNormal, lightDir), 0.0);
    
    // Применить диффузное освещение к цвету текстуры
    vec3 diffuseColor = vec3(0.2, 0.2, 0.2) * diffuseFactor; // Слегка белый цвет при прямом освещении
    
    // Смешать цвет текстуры с цветом освещения
    vec3 finalColor = distanceMod * textureColor.rgb;
    
    // Минимальное освещение для темной стороны
    float dotProduct = max(dot(normalizedNormal, lightDir), 0.05);
    finalColor *= dotProduct;
    finalColor += diffuseColor;
    
    // Использовать альфа-канал из текстуры
    FragColor = vec4(finalColor, textureColor.a);
}