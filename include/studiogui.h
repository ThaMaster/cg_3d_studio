#ifndef STUDIOGUI_H
#define STUDIOGUI_H

#include "studio3d.h"

/**
 * StudioGui is simply a namespace where all the main components 
 * of the 3D studios graphical user interface are implemented. 
 * This seperates the implementation of the GUI somewhat from the
 * main program.
 * 
 * It utilizes the ImGui library which can be found in the 
 * following link:
 * 
 * https://github.com/ocornut/imgui
 * 
 * Author: Christoffer Nordlander (c20cnr@cs.umu.se)
 * 
 * Version information:
 *      2024-01-08: v1.0, first version.
 */

#define VERSION "3D Studio v1.0.0"

namespace StudioGui {

    void mainMenuBar(GLFWwindow*, Studio3D::WindowInfo&, WorldContext&);
    void sceneWindow(bool&, WorldContext&);
    void aboutPopupModal(bool&);
    void objMatWindow(bool&, Object&);
    void objInfWindow(bool&, std::string, Object::ObjectInfo&);
    void camWindow(bool&, WorldContext::CameraInfo&, glm::vec3, glm::vec3);
    void keyRefWindow(bool&);
    void showStudioOverlay(bool&, WorldContext);
    void showLightSourcesWindow(bool&, WorldContext&);
    void logWindow(bool&, Logger&);
    void settingsWindow(bool&, WorldContext&);

}

#endif