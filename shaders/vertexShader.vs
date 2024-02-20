#version 400 core
layout(location = 0) in vec3 inPosition;   // Входная позиция
layout(location = 1) in vec3 inNormal;     // Входная нормаль
layout(location = 2) in vec2 inTexCoord;   // Входная позиция текстуры

out vec3 fragPos;      // Выходная позиция во фрагментный шейдер
out vec3 fragNormal;   // Выходная нормаль во фрагментный шейдер
out vec2 texCoord;     // Выходная позиция текстуры во фрагментный шейдер

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Расчет позиции в мировых координатах
    vec4 worldPos = model * vec4(inPosition, 1.0);
    
    // Расчет позиции в обзорных координатах
    vec4 viewPos = view * worldPos;
    
    // Расчет нормали в мировых координатах
    vec3 worldNormal = normalize(mat3(view * model) * inNormal);
    
    // Установка значений
    fragPos = viewPos.xyz;
    fragNormal = worldNormal;
    texCoord = inTexCoord;
    
    // Расчет финальной позиции в обзорных координатах
    gl_Position = projection * viewPos;
}