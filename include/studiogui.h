#ifndef STUDIOGUI_H
#define STUDIOGUI_H

#include "openglwindow.h"

namespace StudioGui {
    void mainMenuBar(GLFWwindow*, OpenGLWindow::objectInfo, OpenGLWindow::windowInfo&);
    void aboutPopupModal(bool&);
    void objTransWindow(bool&, bool&);
    void objInfWindow(bool&, std::string, std::string, OpenGLWindow::objectInfo);
    void camWindow(bool&, OpenGLWindow::cameraInfo&, glm::vec3, glm::vec3);
    void keyRefWindow(bool&);
    void showStudioOverlay(bool&, std::string, OpenGLWindow::objectInfo);
    void logWindow(bool&, Logger log);
}

#endif