#include "studiogui.h"
namespace StudioGui {

    void mainMenuBar(GLFWwindow *glfwWindow, OpenGLWindow::windowInfo &wInfo, WorldContext &wContext)
    {
        if (ImGui::BeginMainMenuBar())
        {
            bool noObjects = wContext.objects.size() == 0;
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Open")) { wInfo.openFileDialog = true; }  
                if(noObjects) ImGui::BeginDisabled();
                if(ImGui::MenuItem("Reset Scene")) { wContext.clearObjects(); }
                if(noObjects) ImGui::EndDisabled();
                if(ImGui::MenuItem("Settings")) {}
                ImGui::Separator();         
                if(ImGui::MenuItem("Quit")) { glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE); }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("View"))
            {
                ImGui::SeparatorText("Object");
                if(noObjects) ImGui::BeginDisabled();
                if(ImGui::MenuItem("Object Transformation", "F1", wInfo.showObjTransWindow)) { wInfo.showObjTransWindow = !wInfo.showObjTransWindow; }
                if(ImGui::MenuItem("Object Material", "F2", wInfo.showObjMatWindow)) { wInfo.showObjMatWindow = !wInfo.showObjMatWindow; }
                if(ImGui::MenuItem("Object Information", "F3", wInfo.showObjInfWindow)) { wInfo.showObjInfWindow = !wInfo.showObjInfWindow; }
                if(noObjects) ImGui::EndDisabled();
                ImGui::SeparatorText("Camera");
                if(ImGui::MenuItem("Camera Information", "F4", wInfo.showCamWindow)) { wInfo.showCamWindow = !wInfo.showCamWindow; }
                ImGui::SeparatorText("Lightning");
                if(ImGui::MenuItem("Light Sources", "F", wInfo.showLightSourcesWindow)) { wInfo.showLightSourcesWindow = !wInfo.showLightSourcesWindow; }
                ImGui::SeparatorText("Studio");
                if(ImGui::MenuItem("Studio Overlay", NULL, wInfo.showOverlay)) { wInfo.showOverlay = !wInfo.showOverlay; }
                if(ImGui::MenuItem("Log Window", "F10", wInfo.showLogWindow)) { wInfo.showLogWindow = ! wInfo.showLogWindow; }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Help")) 
            {
                if(ImGui::MenuItem("Introduction")) {}
                if(ImGui::MenuItem("Keyboard Shortcuts Reference", "F9", wInfo.showKeyRefWindow)) { wInfo.showKeyRefWindow = !wInfo.showKeyRefWindow; }
                ImGui::Separator();   
                if(ImGui::MenuItem("About")) wInfo.aboutOpen = true;
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        aboutPopupModal(wInfo.aboutOpen);
    }

    // Contains only PLACEHOLDER for now.
    void objTransWindow(bool &showWindow, bool &reset)
    {
        if(showWindow) {
            ImGui::Begin("Object Transformation", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SeparatorText("Translation");
            ImGui::Text("--placeholder--");
            ImGui::SeparatorText("Scaling");
            ImGui::Text("--placeholder--");
            ImGui::SeparatorText("Rotation");
            ImGui::Text("--placeholder--");
            ImGui::SeparatorText("Shearing");
            ImGui::Text("--placeholder--");
            ImGui::SeparatorText("Reset");
            if(ImGui::Button("Reset Object Transformations")) { reset = true; }
            ImGui::End();
        }
    }

    void objInfWindow(bool &showWindow, std::string objFileName, std::string objFilePath, Object::objectInfo& oInfo)
    {
        if(showWindow) {
            ImGui::Begin("Object Information", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SeparatorText("Object Information");
            if(oInfo.objectLoaded) {
                char fileName[objFileName.length()+1];
                char filePath[objFilePath.length()+objFileName.length()+2];
                strcpy(fileName, objFileName.c_str());
                strcpy(filePath, (objFilePath+"/"+objFileName).c_str());
                ImGui::Text("Object File Name: %s", fileName);
                ImGui::Text("File Path: %s", filePath);
                ImGui::Separator();
                ImGui::Text("Shapes:");
                ImGui::SameLine(200); ImGui::Text("%d", (int)oInfo.nShapes);
                ImGui::Text("Vertices:");
                ImGui::SameLine(200); ImGui::Text("%d", oInfo.nVertices);
                ImGui::Text("Indices:");
                ImGui::SameLine(200); ImGui::Text("%d", oInfo.nIndices);
                ImGui::Text("Faces:");
                ImGui::SameLine(200); ImGui::Text("%d", oInfo.nFaces);
                ImGui::Text("Normals:");
                ImGui::SameLine(200); ImGui::Text("%d", oInfo.nVertexNormals);
                ImGui::Text("Texture Coordinates:");
                ImGui::SameLine(200); ImGui::Text("%d", oInfo.nTexCoords);
                ImGui::Text("Color Values:");
                ImGui::SameLine(200); ImGui::Text("%d", oInfo.nColors);
                ImGui::Checkbox("Wireframe Mode", &oInfo.showWireFrame);
            }
            ImGui::End();
        }
    }

    // Contains only PLACEHOLDER for now.
    void objMatWindow(bool &showWindow, Object &object)
    {
        if(showWindow) {
            static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
            ImGui::Begin("Object Material", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SeparatorText("Shininess");
            ImGui::Text("Alpha: %d", object.matAlpha);
            ImGui::SliderInt("Alpha", &object.matAlpha, 2, 16, "%d", flags);
            ImGui::End();
        }
    }

    void aboutPopupModal(bool &aboutOpen)
    {
        if(aboutOpen) {
            ImGui::OpenPopup("About");    
            // Always center this window when appearing
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("About", &aboutOpen, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Placeholder");
                ImGui::Separator();
                //static int unused_i = 0;
                //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0")
                if (ImGui::Button("OK", ImVec2(120, 0))) { 
                    aboutOpen = false;
                    ImGui::CloseCurrentPopup(); 
                }
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }
    }

    void camWindow(bool &showWindow, WorldContext::cameraInfo &cInfo, glm::vec3 defaultZero, glm::vec3 defaultRef)
    {
        if(showWindow) {
            static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
            ImGui::Begin("Camera", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SeparatorText("Positions");
            ImGui::Text("Camera Position (pZero): ");
            ImGui::PushItemWidth(80);
            ImGui::Text("X: %.3f", cInfo.pZero.x); ImGui::SameLine(); 
            ImGui::Text("Y: %.3f", cInfo.pZero.y); ImGui::SameLine(); 
            ImGui::Text("Z: %.3f", cInfo.pZero.z);
            ImGui::PopItemWidth();
            ImGui::Text("Reference Point (pRef): ");
            ImGui::PushItemWidth(100);
            ImGui::Text("X: %.3f", cInfo.pRef.x); ImGui::SameLine();
            ImGui::Text("Y: %.3f", cInfo.pRef.y); ImGui::SameLine();
            ImGui::Text("Z: %.3f", cInfo.pRef.z);
            ImGui::PopItemWidth();
            ImGui::SeparatorText("Camera Transformations");
            ImGui::Text("Translation");
            ImGui::Text("Rotation");
            if(ImGui::Button("Reset Camera Position")) { cInfo.pZero = defaultZero; }
            if(ImGui::Button("Reset Reference Point")) { cInfo.pRef = defaultRef; }
            ImGui::SeparatorText("Projection");
            const char* items[] = {"Perspective", "Parallel" };
            static int proj_current_idx = 0;
            if (ImGui::Combo("Projection type", &proj_current_idx, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items)));
            if (proj_current_idx == 0) {
                cInfo.perspProj = true;
                ImGui::SliderFloat("Field of view",&cInfo.fov, 20.0f, 160.0f, "%1.0f", flags);
                ImGui::SliderFloat("Far",&cInfo.farPlane, 1.0f, 1000.0f, "%1.0f", flags);
            }
            if (proj_current_idx == 1) {
                cInfo.perspProj = false;
                ImGui::SliderFloat("Top",&cInfo.top, 1.0f, 100.0f, "%.1f", flags);
                ImGui::SliderFloat("Far",&cInfo.farPlane, 1.0f, 1000.0f, "%1.0f", flags);
                ImGui::SliderFloat("Oblique scale",&cInfo.obliqueScale, 0.0f, 1.0f, "%.1f", flags);
                ImGui::SliderAngle("Oblique angle",&cInfo.obliqueAngleRad, 15, 75, "%1.0f", flags);
            }
            ImGui::End();
        }
    }

    // Contains only PLACEHOLDER for now.
    void showLightSourcesWindow(bool &showWindow, LightSource &lightSource)
    {
        if(showWindow) {
            static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
            ImGui::Begin("Light Sources", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SeparatorText("Position");
            ImGui::Text("Light Position: ");
            ImGui::PushItemWidth(100);
            ImGui::Text("X: %.3f", lightSource.position.x); ImGui::SameLine(); 
            ImGui::Text("Y: %.3f", lightSource.position.y); ImGui::SameLine(); 
            ImGui::Text("Z: %.3f", lightSource.position.z);
            ImGui::PopItemWidth();
            ImGui::SliderFloat("X",&lightSource.position.x, -10.0f, 10.0f, "%.2f", flags);
            ImGui::SliderFloat("Y",&lightSource.position.y, -10.0f, 10.0f, "%.2f", flags);
            ImGui::SliderFloat("Z",&lightSource.position.z, -10.0f, 10.0f, "%.2f", flags);
            if(ImGui::Button("Reset Light Direction")) { lightSource.resetDir(); }
            ImGui::SeparatorText("Color");
            ImGui::PushItemWidth(100);
            ImGui::Text("R: %.3f", lightSource.color.x); ImGui::SameLine(); 
            ImGui::Text("G: %.3f", lightSource.color.y); ImGui::SameLine(); 
            ImGui::Text("B: %.3f", lightSource.color.z); ImGui::SameLine(); 
            ImGui::Text("A: %.3f", lightSource.color.w);
            ImGui::PopItemWidth();
            ImGui::SliderFloat("R",&lightSource.color.x, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("G",&lightSource.color.y, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("B",&lightSource.color.z, 0.0f, 1.0f, "%.2f", flags);
            if(ImGui::Button("Reset Light Color")) { lightSource.resetColor(); }
            ImGui::End();
        }
    }
    
    void keyRefWindow(bool &showWindow)
    {
        if(showWindow){ 
            ImGui::Begin("Keyboard Shortcut Reference", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SeparatorText("Shortcuts");
            ImGui::Text("--placeholder");
            ImGui::End();
        }
    }

    void showStudioOverlay(bool &showOverlay, std::string objFileName, Object::objectInfo oInfo)
    {
        if(showOverlay) {
            static int location = 0;
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos;
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            window_flags |= ImGuiWindowFlags_NoMove;
            
            ImGui::SetNextWindowBgAlpha(0.35f);
            if(ImGui::Begin("Overlay", &showOverlay, window_flags))
            {
                ImGui::SeparatorText("3D Studio v0.0.1");
                if(oInfo.objectLoaded) {
                    char fileName[objFileName.length()+1];
                    strcpy(fileName, objFileName.c_str());
                    ImGui::Text("Loaded object: %s", fileName);
                }
                if (ImGui::BeginPopupContextWindow())
                {
                    if (ImGui::MenuItem("Top-left (default)",     NULL, location == 0)) location = 0;
                    if (ImGui::MenuItem("Top-right",    NULL, location == 1)) location = 1;
                    if (ImGui::MenuItem("Bottom-left",  NULL, location == 2)) location = 2;
                    if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
                    if (showOverlay && ImGui::MenuItem("Close")) showOverlay = false;
                    ImGui::EndPopup();
                }
            }
            ImGui::End();
        }
    }

    void showEmptyStudioOverlay(bool& showOverlay) 
    {
        if(showOverlay) {
            static int location = 0;
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos;
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            window_flags |= ImGuiWindowFlags_NoMove;
            
            ImGui::SetNextWindowBgAlpha(0.35f);
            if(ImGui::Begin("Overlay", &showOverlay, window_flags))
            {
                ImGui::SeparatorText("3D Studio v0.0.1");
                if (ImGui::BeginPopupContextWindow())
                {
                    if (ImGui::MenuItem("Top-left (default)",     NULL, location == 0)) location = 0;
                    if (ImGui::MenuItem("Top-right",    NULL, location == 1)) location = 1;
                    if (ImGui::MenuItem("Bottom-left",  NULL, location == 2)) location = 2;
                    if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
                    if (showOverlay && ImGui::MenuItem("Close")) showOverlay = false;
                    ImGui::EndPopup();
                }
            }
            ImGui::End();
        }
    }


    void logWindow(bool &showWindow, Logger &log)
    {
        if(showWindow) {
            ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
            ImGui::Begin("Example: Log", &showWindow);

            // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
            bool clearBuf = ImGui::Button("Clear"); ImGui::SameLine();
            bool copyLog = ImGui::Button("Copy");

            ImGui::Separator();

            if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
            {
                if (clearBuf) log.clear();
                if (copyLog) ImGui::LogToClipboard();

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

                const char* buf_begin = log.textBuffer.begin();
                const char* buf_end = log.textBuffer.end();

                ImGui::TextUnformatted(buf_begin, buf_end);
                ImGui::PopStyleVar();

                // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
                // Using a scrollbar or mouse-wheel will take away from the bottom edge.
                if (log.autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
            }
            ImGui::EndChild();
            ImGui::End();
        }
        
    }
}