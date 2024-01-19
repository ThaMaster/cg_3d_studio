#pragma once

#include "studio3d.h"
#include "loader.h"
#include "stb_image.h"
#include <glm/gtx/string_cast.hpp>

/**
 * This class renders the objects and handles the OpenGL shader files such 
 * as loading the files, loading objects, loading textures and more.
 * It inherits the Studio3D object and overwrites certain functions in 
 * order to make them available in the main program.
 * 
 * Author: Christoffer Nordlander (c20cnr@cs.umu.se)
 * 
 * Version information:
 *      2024-01-08: v1.0, first version.
 */
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
        string loadObjectFromGui(string, string) override;
        string loadTextureFromGui(string, string, int) override;

    private:
        GLuint program;
        Loader loader;
        bool objectParseSuccess;

        void debugShader(void) const;
        void loadGeometry(string, string);
        void resetTransformations(int);
        string loadTexture(string, string, GLuint&, int);
};
