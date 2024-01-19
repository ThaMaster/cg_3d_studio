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

/**
 * This is the main class of the whole program. This class
 * compunicates with many classes in order for loadning new
 * objects to the scene, rendering these objects, transforming
 * these objects and more. This is essentially the center
 * of the program.
 * 
 * Author: Christoffer Nordlnader (c20cnr@cs.umu.se)
 * 
 * Version information:
 *      2024-01-08: v1.0, first version.
 */
class Studio3D
{
    public:
        struct WindowInfo {
            bool aboutOpen = false;
            bool showOverlay = true;
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

        Studio3D(string title, int width, int height);
        ~Studio3D();

        GLFWwindow* window() const;
        void start();

        virtual void errorCallback(int error, const char* desc);
        virtual void resizeCallback(GLFWwindow* window, int width, int height);
        virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        virtual void initialize() = 0;
        virtual void display() = 0;
        virtual void updateObject(int) = 0;
        virtual void updateCamera() = 0;
        virtual void updateLight() = 0;
        virtual string loadObjectFromGui(string, string) = 0;
        virtual string loadTextureFromGui(string, string, int) = 0;

    protected:
        bool checkOpenGLError() const;
        int width() const;
        int height() const;
        float getAspectRatio();

        string readShaderSource(const string shaderFile) const;
        GLuint initProgram(const string vShaderFile, const string fShaderFile) const;

        void reshape(const int width, const int height) const;
        WorldContext wContext = WorldContext();

    private:
        int windowWidth = 0;
        int windowHeight = 0;

        string objFileName;
        string objFilePath;
        string texName;
        string texPath;  

        GLFWwindow* glfwWindow;
        Logger log = Logger();
        
        void DrawGui();
        void handleMouseInput(); 
        void openObjectFile();
        void openTextureFile();
};
