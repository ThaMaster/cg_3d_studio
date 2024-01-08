#include "renderer.h"
#include "glfwcallbackmanager.h"

Studio3D* glfwCallbackManager::app = nullptr;

int main(int argc, char **argv)
{
    Renderer app("3D Studio", 1024, 768);
    glfwCallbackManager::initCallbacks(&app);
    app.initialize();

    app.start();
}
