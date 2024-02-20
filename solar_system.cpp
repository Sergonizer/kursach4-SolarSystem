#include "solar_system.h"

SolarSystem::SolarSystem(float size_scale, float sun_size_scale, float orbit_scale)
    : planetShader(Shader("shaders/vertexShader.vs", "shaders/fragmentShader.fs"))
    , sunShader(Shader("shaders/vertexShader.vs", "shaders/fragmentShaderNoLight.fs"))
    , orbitShader(Shader("shaders/vertexShader.vs", "shaders/colorFragmentShader.fs"))
    , ringShader(Shader("shaders/vertexShader.vs", "shaders/fragmentShaderNoLight.fs"))
{
    // Добавляем Солнце
    planets.emplace_back(sun_size_scale * 695990, orbit_scale * 0, 1, 500, 0, "images/sun.tga");

    // Добавляем планеты
    planets.emplace_back(size_scale * 2440, orbit_scale * 58000000, 88, 59, 0, "images/mercury.tga");
    planets.emplace_back(size_scale * 6050, orbit_scale * 108000000, 225, 243, 177.3f, "images/venus.tga");
    planets.emplace_back(size_scale * 6400, orbit_scale * 150000000, 365, 1, 23.4f, "images/earth.tga");
    planets.emplace_back(size_scale * 3400, orbit_scale * 228000000, 686, 1.03f, 25.2f, "images/mars.tga");
    planets.emplace_back(size_scale * 71000, orbit_scale * 778000000, 4332, 0.4139f, 3.1f, "images/jupiter.tga");
    planets.emplace_back(size_scale * 60000, orbit_scale * 1430000000, 10759, 0.44375f, 26.7f, "images/saturn.tga", size_scale * 58232, 1.5f * size_scale * 58232, "images/saturnrings.tga");
    planets.emplace_back(size_scale * 25900, orbit_scale * 2870000000, 30685, 0.718056f, 97.8f, "images/uranus.tga");
    planets.emplace_back(size_scale * 24750, orbit_scale * 4500000000, 60188, 0.6713f, 28.3f, "images/neptune.tga");

    // Добавляем Луну Земле
    planets.at(3).AddMoon();
}

// Обновление позиций планет и их спутников
void SolarSystem::Update(float elapsedTime)
{
    for (auto& planet : planets)
        planet.Update(elapsedTime);
}

// Отрисовка солнечной системы
void SolarSystem::Draw(Camera camera)
{
    // Отрисовка Солнца
    sunShader.Use();
    sunShader.SetMat4("view", camera.GetViewMatrix());
    sunShader.SetMat4("projection", camera.GetProjectionMatrix());

    // Отрисовка планет
    planetShader.Use();
    planetShader.SetMat4("view", camera.GetViewMatrix());
    planetShader.SetMat4("projection", camera.GetProjectionMatrix());
    planetShader.SetVec3("lightPos", planets.at(0).GetPosition());

    // Отрисовка орбит
    orbitShader.Use();
    orbitShader.SetMat4("view", camera.GetViewMatrix());
    orbitShader.SetMat4("projection", camera.GetProjectionMatrix());

    // Отрисовка колец
    ringShader.Use();
    ringShader.SetMat4("view", camera.GetViewMatrix());
    ringShader.SetMat4("projection", camera.GetProjectionMatrix());

    // Отрисовка Солнца
    planets.at(0).Draw(sunShader);

    // Отрисовка планет и колец
    for (int i = 1; i < planets.size(); i++)
    {
        planets.at(i).Draw(planetShader);
        if (drawOrbitsOn)
            planets.at(i).DrawOrbit(orbitShader);
        planets.at(i).DrawRings(ringShader);
    }
}

// Получение списка планет
std::vector<Planet>& SolarSystem::GetPlanets()
{
    return planets;
}