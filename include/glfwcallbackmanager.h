/**
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#include "studio3d.h"

// Class for bridging between the C callback functions in glfw and C++ object
class glfwCallbackManager
{
    static Studio3D* app;

    static void errorCallback(int error, const char* description)
    {
        if(app) 
            app->errorCallback(error,description);
    }
    
    static void resizeCallback(GLFWwindow* window, int width, int height)
    {
        if(app) 
            app->resizeCallback(window,width,height);
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(app)
            app->keyCallback(window, key, scancode, action, mods);
    }
    
public:
    static void initCallbacks(Studio3D* glfwapp)
    {
        app = glfwapp;
        
        glfwSetErrorCallback(errorCallback);
        glfwSetFramebufferSizeCallback(app->window() , resizeCallback);
        glfwSetKeyCallback(app->window(), keyCallback);
    }
};
