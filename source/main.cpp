#include "renderer.h"
#include "glfwcallbackmanager.h"

/**
 * The main function for starting the whole 3D Studio program.
 * This program is capable of rendering object files
 * with functions such as:
 * 
 *      - Camera Navigation
 *      - Multiple projection types
 *      - Object Transformations
 *      - Adjustments of lightning
 *      - Material Files
 *      - Texture loading
 * 
 * This program utilizes the OpenGL library to handle the
 * rendering of objects and other features. The graphical
 * user interface (GUI) of the program were developed using
 * the ImGui library which can be found here:
 * 
 * https://github.com/ocornut/imgui
 * 
 * Author: Christoffer Nordlander (c20cnr@cs.umu.se)
 * 
 * Version information:
 *      2024-01-08: v1.0, first version.
 */

Studio3D* glfwCallbackManager::app = nullptr;

int main(int argc, char **argv)
{
    Renderer app("3D Studio", 1024, 768);
    glfwCallbackManager::initCallbacks(&app);
    app.initialize();

    app.start();
}
