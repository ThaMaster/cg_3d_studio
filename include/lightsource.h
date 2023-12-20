#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/component_wise.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class LightSource
{
    public:

    glm::vec4 position;
    glm::vec4 color;

    glm::vec4 defaultPos;
    glm::vec4 defaultColor;

    LightSource(glm::vec4 pos, glm::vec4 color)
    {
        LightSource::position = pos;
        LightSource::defaultPos = pos;
        LightSource::color = color;
        LightSource::defaultColor = color;

    }

    void resetDir() 
    {
        LightSource::position = defaultPos;
    }

    void resetColor() 
    {
        LightSource::color = defaultColor;
    }
};

#endif