/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/component_wise.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "3dstudio.h"


class OpenGLWindow
{
public:
    OpenGLWindow(std::string title, int width, int height);
    ~OpenGLWindow();

    GLFWwindow* window() const;
    virtual void errorCallback(int error, const char* desc);
    virtual void resizeCallback(GLFWwindow* window, int width, int height);
    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void start();
    virtual void initialize() = 0;
    virtual void display() = 0;
    virtual void transform(glm::vec3, glm::vec3, float) = 0;
    void displayNow();

    const float ROT_SPEED = 10.0f;
    const float TRA_SPEED = 0.1f;
    const float SCA_INC_SPEED = 1.1f;
    const float SCA_DEC_SPEED = 0.9f;


protected:
    bool checkOpenGLError() const;
    int width() const;
    int height() const;

    glm::vec3 rVals = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 tVals = glm::vec3(0.0f,0.0f,0.0f);
    float scVal = 0.0f;

    std::string readShaderSource(const std::string shaderFile) const;
    GLuint initProgram(const std::string vShaderFile, const std::string fShaderFile) const;

    void reshape(const int width, const int height) const;

private:
    GLFWwindow* glfwWindow;
    int windowWidth = 0;
    int windowHeight = 0;
};