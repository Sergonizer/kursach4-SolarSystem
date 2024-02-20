#ifndef OBJ_H
#define OBJ_H

#include "common.h"
#include "shader.h"
#include "texture.h"
#include "vao_vbo.h"

class Object
{
public:
    Object(const char* texturePath) : texture(texturePath) {}
    virtual void Draw(Shader shader) = 0;

protected:
    Texture texture;
    VaoVbo vaoVbo;
};

#endif