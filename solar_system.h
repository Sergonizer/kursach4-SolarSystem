#ifndef SOLSYS_H
#define SOLSYS_H

#include "common.h"
#include "planet.h"
#include "shader.h"
#include "camera.h"

class SolarSystem
{
public:
    SolarSystem() = default;
    SolarSystem(float size_scale, float sun_size_scale, float orbit_scale);
    void Update(float elapsedTime);
    void Draw(Camera camera);

    std::vector<Planet>& GetPlanets();

    bool drawOrbitsOn = true;

private:
    std::vector<Planet> planets; 
    Shader planetShader, sunShader, orbitShader, ringShader;
};

#endif