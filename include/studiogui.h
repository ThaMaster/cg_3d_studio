#ifndef STUDIOGUI_H
#define STUDIOGUI_H

#include "studio3d.h"

#define VERSION "3D Studio v0.4.2"

namespace StudioGui {

    void mainMenuBar(GLFWwindow*, Studio3D::windowInfo&, WorldContext&);
    void sceneWindow(bool&, WorldContext&);
    void aboutPopupModal(bool&);
    void objTransWindow(bool&, WorldContext&);
    void objMatWindow(bool&, Object&);
    void objInfWindow(bool&, std::string, Object::objectInfo&);
    void camWindow(bool&, WorldContext::cameraInfo&, glm::vec3, glm::vec3);
    void keyRefWindow(bool&);
    void showStudioOverlay(bool&, WorldContext);
    void showLightSourcesWindow(bool&, LightSource&, WorldContext&);
    void logWindow(bool&, Logger&);
    void settingsWindow(bool&, WorldContext&);

}

#endif