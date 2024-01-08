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

#include "logger.h"
#include "worldcontext.h"

class Studio3D
{
    public:
        struct windowInfo {
            bool aboutOpen = false;
            bool showOverlay = true;
            bool showObjTransWindow = false;
            bool showObjInfWindow = false;
            bool showCamWindow = false;
            bool showKeyRefWindow = false;
            bool openObjFileDialog = false;
            bool openTexFileDialog = false;
            bool showLogWindow = false;
            bool showLightSourcesWindow = false;
            bool showObjMatWindow = false;
            bool showSettingsWindow = false;
            bool showSceneWindow = false;
        } wInfo;

        Studio3D(std::string title, int width, int height);
        ~Studio3D();

        GLFWwindow* window() const;
        virtual void errorCallback(int error, const char* desc);
        virtual void resizeCallback(GLFWwindow* window, int width, int height);
        virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        void start();
        virtual void initialize() = 0;
        virtual void display() = 0;
        virtual void updateObject(int) = 0;
        virtual void updateCamera() = 0;
        virtual void updateLight() = 0;
        virtual void updateMaterial(int) = 0;

        virtual std::string loadObjectFromGui(std::string) = 0;
        virtual std::string loadTextureFromGui(std::string, int) = 0;

        void displayNow();

    protected:
        bool checkOpenGLError() const;
        int width() const;
        int height() const;
        float getAspectRatio();

        std::string readShaderSource(const std::string shaderFile) const;
        GLuint initProgram(const std::string vShaderFile, const std::string fShaderFile) const;

        void reshape(const int width, const int height) const;
        WorldContext wContext = WorldContext();

    private:
        Logger log = Logger();
        std::string objFileName;
        std::string objFilePath;
        std::string texFileName;
        std::string texFilePath;  

        void DrawGui();
        void handleMouseInput(); 
        GLFWwindow* glfwWindow;
        int windowWidth = 0;
        int windowHeight = 0;

        void openObjectFile();
        void openTextureFile();
};
