#ifndef STUDIOGUI_H
#define STUDIOGUI_H

#include "openglwindow.h"

namespace StudioGui {
    void mainMenuBar(GLFWwindow*, OpenGLWindow::windowInfo&, vector<Object>);
    void aboutPopupModal(bool&);
    void objTransWindow(bool&, bool&);
    void objInfWindow(bool&, std::string, std::string, Object::objectInfo);
    void camWindow(bool&, WorldContext::cameraInfo&, glm::vec3, glm::vec3);
    void keyRefWindow(bool&);
    void showStudioOverlay(bool&, std::string, Object::objectInfo);
    void showEmptyStudioOverlay(bool&);

    void logWindow(bool&, Logger&);
}

#endif