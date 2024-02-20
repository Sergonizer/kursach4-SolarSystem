#ifndef SKYBOX_H
#define SKYBOX_H

#include "object.h"

class SkyBox : public Object
{
public:
    SkyBox() = default;
    SkyBox(const char* texturePath);

    void Draw(Shader shader) override;
};

#endif