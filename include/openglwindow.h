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

#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "3dstudio.h"

const float pi_f = 3.1415926f;

class OpenGLWindow
{
    public:
        struct objectInfo {
            glm::vec3 cVals = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 rVals = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 tVals = glm::vec3(0.0f, 0.0f, 0.0f);
            float scVal = 0.0f;
            bool reset = false;
        } oInfo;

        struct cameraInfo {
            glm::vec3 pZero = glm::vec3(0.0f, 0.0f, 2.0f);
            glm::vec3 pRef = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 upVec = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 camOffset = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 camRotOffset = glm::vec3(0.0f, 0.0f, 0.0f);
            float fov = 60.0f;
            float farPlane = 500.0f;
            float nearPlane = 0.1f;
            float top = 1.0f;
            float obliqueScale = 0.0f;
            float obliqueAngleRad = pi_f/4.0f;
            bool perspProj = true;
        } cInfo;

        OpenGLWindow(std::string title, int width, int height);
        ~OpenGLWindow();

        GLFWwindow* window() const;
        virtual void errorCallback(int error, const char* desc);
        virtual void resizeCallback(GLFWwindow* window, int width, int height);
        virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        void start();
        virtual void initialize() = 0;
        virtual void display() = 0;
        virtual void updateObject() = 0;
        virtual void loadObjectFromGui(std::string) = 0;
        virtual void reset() = 0;
        void displayNow();

        const float ROT_SPEED = 10.0f;
        const float TRA_SPEED = 0.1f;
        const float SCA_INC_SPEED = 1.1f;
        const float SCA_DEC_SPEED = 0.9f;
        const float CAM_SPEED = 0.1f;

    protected:
        bool checkOpenGLError() const;
        int width() const;
        int height() const;
        float getAspectRatio();

        std::string readShaderSource(const std::string shaderFile) const;
        GLuint initProgram(const std::string vShaderFile, const std::string fShaderFile) const;

        void reshape(const int width, const int height) const;

    private:
        bool aboutOpen = false;
        bool showOverlay = true;

        bool showObjTransWindow = false;
        bool showObjInfWindow = false;
        bool showCamWindow = true;

        void DrawGui();
        void handleMouseInput(); 
        GLFWwindow* glfwWindow;
        int windowWidth = 0;
        int windowHeight = 0;

        void showStudioOverlay(bool *);
        void mainMenuBar();
        void aboutPopupModal();
        void objTransWindow();
        void objInfWindow();
        void camWindow();
};
