#pragma once

#include "studio3d.h"
#include "loader.h"
#include "stb_image.h"

class Renderer : public Studio3D
{
public:
    template<typename... ARGS>
    Renderer(ARGS&&... args) : Studio3D{ std::forward<ARGS>(args)... }
    {}
    
    void initialize();
    virtual void display() override;
    void updateObject(int) override;
    void updateCamera() override;
    void updateLight() override;
    void updateMaterial(int) override;
    string loadObjectFromGui(string) override;
    string loadTextureFromGui(string, int) override;

private:
    GLuint program;
    Loader loader;
    bool objectParseSuccess;

    void debugShader(void) const;
    void loadGeometry(string);
    void resetTransformations(int);
    string loadTexture(string, GLuint&, int);
};
