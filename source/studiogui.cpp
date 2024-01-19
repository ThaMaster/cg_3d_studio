#include "studiogui.h"

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
namespace StudioGui 
{
    /**
     * Function for creating the menu bar at the top of the program.
     * Depending on if a object has been loaded, some of the menu 
     * elements will not be available.
     * 
     * @param glfwWindow: The window of the program.
     * @param wInfo: The window info, shows which windows should be open and not.
     * @param wContext: The world context, holds information of the scene.
     */
    void mainMenuBar(GLFWwindow *glfwWindow, Studio3D::WindowInfo &wInfo, WorldContext &wContext)
    {
        if (ImGui::BeginMainMenuBar())
        {
            bool noObjects = wContext.objects.size() == 0;
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Load New Object")) { wInfo.openObjFileDialog = true; }  
                if(noObjects) ImGui::BeginDisabled();
                if(ImGui::MenuItem("Load New Texture")) { wInfo.openTexFileDialog = true; }  
                if(ImGui::MenuItem("Reset Scene")) { wContext.clearObjects(); }
                if(noObjects) ImGui::EndDisabled();
                if(ImGui::MenuItem("Settings", NULL, wInfo.showSettingsWindow)) { wInfo.showSettingsWindow = !wInfo.showSettingsWindow; }
                ImGui::Separator();
                if(ImGui::MenuItem("Quit")) { glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE); }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("View"))
            {
                ImGui::SeparatorText("Scene");
                if(ImGui::MenuItem("Loaded Objects", "F1", wInfo.showSceneWindow)) { wInfo.showSceneWindow = !wInfo.showSceneWindow; }
                ImGui::SeparatorText("Object");
                if(noObjects) ImGui::BeginDisabled();
                if(ImGui::MenuItem("Object Material", "F3", wInfo.showObjMatWindow)) { wInfo.showObjMatWindow = !wInfo.showObjMatWindow; }
                if(ImGui::MenuItem("Object Information", "F4", wInfo.showObjInfWindow)) { wInfo.showObjInfWindow = !wInfo.showObjInfWindow; }
                if(noObjects) ImGui::EndDisabled();
                ImGui::SeparatorText("Camera");
                if(ImGui::MenuItem("Camera Information", "F5", wInfo.showCamWindow)) { wInfo.showCamWindow = !wInfo.showCamWindow; }
                ImGui::SeparatorText("Lightning");
                if(ImGui::MenuItem("Light Sources", "F6", wInfo.showLightSourcesWindow)) { wInfo.showLightSourcesWindow = !wInfo.showLightSourcesWindow; }
                ImGui::SeparatorText("Studio");
                if(ImGui::MenuItem("Studio Overlay", NULL, wInfo.showOverlay)) { wInfo.showOverlay = !wInfo.showOverlay; }
                if(ImGui::MenuItem("Log Window", "F9", wInfo.showLogWindow)) { wInfo.showLogWindow = ! wInfo.showLogWindow; }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Help")) 
            {
                if(ImGui::MenuItem("Keyboard Shortcuts Reference", "F10", wInfo.showKeyRefWindow)) { wInfo.showKeyRefWindow = !wInfo.showKeyRefWindow; }
                ImGui::Separator();   
                if(ImGui::MenuItem("About")) wInfo.aboutOpen = true;
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        aboutPopupModal(wInfo.aboutOpen);
    }

    /**
     * Creates the scene window which contains all the loaded
     * objects and the ability to switch which object that
     * is currently selected. The selected object will 
     * be affected by transformations, loading textures and
     * such.
     * 
     * @param showWindow: Bool if the window should be visible.
     * @param wContext: The world context, contains information regarding
     *                  the loaded objects and currently selected one.
     */
    void sceneWindow(bool& showWindow, WorldContext &wContext) 
    {
        if(showWindow) {
            ImGui::Begin("Scene", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SeparatorText("Current Loaded Objects:");
            if(wContext.objects.size() == 0) {
                ImGui::Text("No objects currently loaded!");
            } else {
                int oIndex = 0;
                for(Object object : wContext.objects) {
                    bool disable = wContext.selectedObject == oIndex;
                    char fileName[object.fileName.length()+1];
                    strcpy(fileName, object.fileName.c_str());

                    ImGui::Text("%s", fileName); ImGui::SameLine(200);
                    std::string buttonLabel = "Select ##" + std::to_string(oIndex);

                    if(disable) ImGui::BeginDisabled();
                    if(ImGui::Button(buttonLabel.c_str())) { wContext.selectedObject = oIndex; }
                    if(disable) ImGui::EndDisabled();
                    oIndex++;
                }
            }

            ImGui::Separator();
            ImGui::End();
        }
    }

    /**
     * Creates a window where information regarding the currently 
     * selected object. It will display information such as
     * number of vertices, indices, file name, path and more. It
     * is also here where one can select to show textures or use
     * wireframe.
     * 
     * @param showWindow: Bool if the window should be visible.
     * @param objFileName: The name of the selected object.
     * @param oInfo: The object information of the selected object.
     */
    void objInfWindow(bool &showWindow, std::string objFileName, Object::ObjectInfo& oInfo)
    {
        if(showWindow) {
            ImGui::Begin("Object Information", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SeparatorText("Object Information");
            if(oInfo.objectLoaded) {
                char fileName[objFileName.length()+1];
                strcpy(fileName, objFileName.c_str());
                ImGui::Text("Object File Name: %s", fileName);
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
                ImGui::Checkbox("Wireframe Mode", &oInfo.showWireFrame);
                bool hasTexture = oInfo.hasTexture;
                if(!hasTexture) ImGui::BeginDisabled();
                ImGui::Checkbox("Show Texture", &oInfo.showTexture);
                if(!hasTexture) ImGui::EndDisabled();

            }
            ImGui::End();
        }
    }

    /**
     * Function for handling the material of an object. Since
     * the specification of the project stated that the
     * material coefficients should be able to change it is
     * in this window where it happens. However, if the object
     * has loaded with materials from its material file one must
     * first disable and use the default materials first.
     * 
     * @param showWindow: Bool if the window should be visible.
     * @param object: The currently selected object.
     */
    void objMatWindow(bool &showWindow, Object &object)
    {
        if(showWindow) {
            static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
            ImGui::Begin("Object Material", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SeparatorText("Shininess");
            ImGui::Text("Alpha: %.3f", object.matAlpha);
            ImGui::SliderFloat("Alpha", &object.matAlpha, 0.0, 50.0, "%.2f", flags);
            if(!object.oInfo.useDefaultMat) ImGui::BeginDisabled();
            ImGui::SeparatorText("Ambient Light");
            ImGui::SliderFloat("R##1", &object.defMat.ka.r, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("G##1", &object.defMat.ka.g, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("B##1", &object.defMat.ka.b, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SeparatorText("Diffuse Light");
            ImGui::SliderFloat("R##2", &object.defMat.kd.r, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("G##2", &object.defMat.kd.g, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("B##2", &object.defMat.kd.b, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SeparatorText("Specular Light");
            ImGui::SliderFloat("R##3", &object.defMat.ks.r, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("G##3", &object.defMat.ks.g, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("B##3", &object.defMat.ks.b, 0.0f, 1.0f, "%.2f", flags);
            if(!object.oInfo.useDefaultMat) ImGui::EndDisabled();
            if(!object.oInfo.hasMaterials) ImGui::BeginDisabled();
            ImGui::Checkbox("Use default material", &object.oInfo.useDefaultMat);
            if(!object.oInfo.hasMaterials) ImGui::EndDisabled();
            ImGui::End();
        }
    }

    /**
     * Function for creating the about modal that holds
     * information regarding creator of the program, 
     * version number, location of project and more.
     * 
     * @param showWindow: Bool if the window should be visible.
     */
    void aboutPopupModal(bool &showWindow)
    {
        if(showWindow) {
            ImGui::OpenPopup("About");    
            // Always center this window when appearing
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("About", &showWindow, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(VERSION);
                ImGui::Separator();
                ImGui::Text("Umeå University project for the Computer Graphics Course");
                ImGui::Text("Developed in C++ utilizing the OpenGL library.");
                ImGui::Separator();
                ImGui::Text("Created by: Christoffer Nordlander");
                ImGui::Text("GitHub Page: https://github.com/ThaMaster/cg_3d_studio");
                ImGui::Text("\n");
                if (ImGui::Button("OK", ImVec2(120, 0))) { 
                    showWindow = false;
                    ImGui::CloseCurrentPopup(); 
                }
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }
    }

    /**
     * Function for creating the window regarding the camera
     * movement and information. One can change the cameras
     * different projection types and position in this window.
     * 
     * @param showWindow: Bool if the window should be visible.
     * @param cInfo: The camera info, containing position and more.
     * @param defaultZero: The default camera position.
     * @param defaultRef: The default cameras reference point.
     */
    void camWindow(bool &showWindow, WorldContext::CameraInfo &cInfo, glm::vec3 defaultZero, glm::vec3 defaultRef)
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

    /**
     * Function for creating the window regarding the handling
     * of light sources. As of right now, the function only alters
     * a single light source. Since the light is directional light
     * one can change the direction, color of light and the color 
     * of the ambient light.
     * 
     * @param showWindow: Bool if the window should be visible.
     * @param wContext: The world context, which holds the light information.
     */
    void showLightSourcesWindow(bool &showWindow, WorldContext& wContext)
    {
        if(showWindow) {
            static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
            ImGui::Begin("Light Sources", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SeparatorText("Position");
            ImGui::Text("Light Position: ");
            ImGui::PushItemWidth(100);
            ImGui::Text("X: %.3f", wContext.light.position.x); ImGui::SameLine(); 
            ImGui::Text("Y: %.3f", wContext.light.position.y); ImGui::SameLine(); 
            ImGui::Text("Z: %.3f", wContext.light.position.z);
            ImGui::PopItemWidth();
            ImGui::SliderFloat("X",&wContext.light.position.x, -10.0f, 10.0f, "%.2f", flags);
            ImGui::SliderFloat("Y",&wContext.light.position.y, -10.0f, 10.0f, "%.2f", flags);
            ImGui::SliderFloat("Z",&wContext.light.position.z, -10.0f, 10.0f, "%.2f", flags);
            if(ImGui::Button("Reset Light Direction")) { wContext.light.resetDir(); }
            ImGui::SeparatorText("Color");
            ImGui::PushItemWidth(100);
            ImGui::Text("R: %.3f", wContext.light.color.x); ImGui::SameLine(); 
            ImGui::Text("G: %.3f", wContext.light.color.y); ImGui::SameLine(); 
            ImGui::Text("B: %.3f", wContext.light.color.z);
            ImGui::PopItemWidth();
            ImGui::SliderFloat("R",&wContext.light.color.x, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("G",&wContext.light.color.y, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("B",&wContext.light.color.z, 0.0f, 1.0f, "%.2f", flags);
            if(ImGui::Button("Reset Light Color")) { wContext.light.resetColor(); }
            ImGui::SeparatorText("Ambient Light Intensity");
            ImGui::PushItemWidth(100);
            ImGui::Text("R: %.3f", wContext.ambientLight.x); ImGui::SameLine(); 
            ImGui::Text("G: %.3f", wContext.ambientLight.y); ImGui::SameLine(); 
            ImGui::Text("B: %.3f", wContext.ambientLight.z);
            ImGui::PopItemWidth();
            ImGui::SliderFloat("R##1",&wContext.ambientLight.x, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("G##1",&wContext.ambientLight.y, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SliderFloat("B##1",&wContext.ambientLight.z, 0.0f, 1.0f, "%.2f", flags);

            if(ImGui::Button("Reset Ambient Intensity")) { wContext.ambientLight = wContext.defaultAmbientLight; }
            ImGui::End();
        }
    }
    
    /**
     * Function for creating the key reference window.
     * This window does not have any functionality but
     * is good to have if any of the keyboard shortcuts
     * are forgotten.
     * 
     * @param showWindow: Bool if the window shoulds be visible.
     */
    void keyRefWindow(bool &showWindow)
    {
        int margin = 250;
        if(showWindow){ 
            ImGui::Begin("Keyboard Shortcuts", &showWindow);
            ImGui::SeparatorText("Camera Controls");
            ImGui::Text("Move camera up:"); ImGui::SameLine(margin); ImGui::Text("'E'");
            ImGui::Text("Move camera down:"); ImGui::SameLine(margin); ImGui::Text("'Q'");
            ImGui::Text("Move camera left:"); ImGui::SameLine(margin); ImGui::Text("'A'");
            ImGui::Text("Move camera right:"); ImGui::SameLine(margin); ImGui::Text("'D'");
            ImGui::Text("Move camera forward:"); ImGui::SameLine(margin); ImGui::Text("'W'");
            ImGui::Text("Move camera backward:"); ImGui::SameLine(margin); ImGui::Text("'S'");
            ImGui::SeparatorText("Translations");
            ImGui::Text("Move object up:"); ImGui::SameLine(margin); ImGui::Text("'I'");
            ImGui::Text("Move object down:"); ImGui::SameLine(margin); ImGui::Text("'K'");
            ImGui::Text("Move object left:"); ImGui::SameLine(margin); ImGui::Text("'J'");
            ImGui::Text("Move object right:"); ImGui::SameLine(margin); ImGui::Text("'L'");
            ImGui::Text("Move object forward:"); ImGui::SameLine(margin); ImGui::Text("'Y'");
            ImGui::Text("Move object backward:"); ImGui::SameLine(margin); ImGui::Text("'H'");

            ImGui::SeparatorText("Rotations");
            ImGui::Text("Rotate object up:"); ImGui::SameLine(margin); ImGui::Text("'Arrow Up'");
            ImGui::Text("Rotate object down:"); ImGui::SameLine(margin); ImGui::Text("'Arrow Down'");
            ImGui::Text("Rotate object left:"); ImGui::SameLine(margin); ImGui::Text("'Arrow Left'");
            ImGui::Text("Rotate object right:"); ImGui::SameLine(margin); ImGui::Text("'Arrow Right'");
            ImGui::Text("Rotate object left on z-axis:"); ImGui::SameLine(margin); ImGui::Text("'COMMA'");
            ImGui::Text("Rotate object right on z-axis:"); ImGui::SameLine(margin); ImGui::Text("'PERIOD'");

            ImGui::SeparatorText("Miscellaneous");
            ImGui::Text("Reset object transformations:"); ImGui::SameLine(margin); ImGui::Text("'R'");

            ImGui::SeparatorText("Window Shortcuts");
            ImGui::Text("Loaded Objects:"); ImGui::SameLine(margin); ImGui::Text("'F1'");
            ImGui::Text("Object Transformation:"); ImGui::SameLine(margin); ImGui::Text("'F2'");
            ImGui::Text("Object Material:"); ImGui::SameLine(margin); ImGui::Text("'F3'");
            ImGui::Text("Object Information:"); ImGui::SameLine(margin); ImGui::Text("'F4'");
            ImGui::Text("Camera Information:"); ImGui::SameLine(margin); ImGui::Text("'F5'");
            ImGui::Text("Light Sources:"); ImGui::SameLine(margin); ImGui::Text("'F6'");
            ImGui::Text("Log Window:"); ImGui::SameLine(margin); ImGui::Text("'F9'");
            ImGui::Text("Keyboard Shortcuts:"); ImGui::SameLine(margin); ImGui::Text("'F10'");

            ImGui::End();
        }
    }

    /**
     * Function for creating the studio overlay. This overlay will
     * display the version of the program and all loaded objects.
     * It will also be displayed here which object that is currently
     * selected.
     * 
     * @param showOverlay: Bool if the overlay should be visible.
     * @param wContext: The world context, holds information regarding objects.
     */
    void showStudioOverlay(bool &showOverlay, WorldContext wContext)
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
                string selected;
                ImGui::SeparatorText(VERSION);
                if(wContext.objects.size() != 0) {
                    ImGui::Text("Loaded object(s):");
                    int oIndex = 0;
                    for(Object object : wContext.objects) {
                        char fileName[object.fileName.length()+1];
                        strcpy(fileName, object.fileName.c_str());
                        if(wContext.selectedObject == oIndex) {
                            selected = "(Selected)";
                        } else {
                            selected = "";
                        }
                        ImGui::Text("%s %s", fileName, selected.c_str());
                        oIndex++;
                    }
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

    /**
     * Function for creating the logwindow. This enables the program
     * to redirect all the output from loading objects and such to
     * be displayed inside the program instead of using the standard
     * output (terminal). One can copy and clear the contents if it
     * should be needed.
     * 
     * @param showWindow: Bool if the window should be visible.
     * @param log: The log object that holds the log entries.
     */
    void logWindow(bool &showWindow, Logger &log)
    {
        if(showWindow) {
            ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
            ImGui::Begin("Studio Log", &showWindow);

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

    /**
     * Function for creating the settings window. This window changes
     * some of the more general values as the user sees fit. It would
     * contain more regarding the visual representation of the GUI but
     * since this is not a mandatory for the project this is enough.
     *
     * @param showWindow: Bool if the window should be visible.
     * @param wContext: The world context, holds the values to be changed.
     */
    void settingsWindow(bool &showWindow, WorldContext &wContext) 
    {
        if(showWindow) {
            static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
            ImGui::Begin("Settings", &showWindow);
            ImGui::SeparatorText("Camera Settings");
            ImGui::Text("Camera Speed");
            ImGui::SliderFloat("##1", &wContext.CAM_SPEED, 0.0f, 1.0f, "%.2f", flags);
            ImGui::SeparatorText("Transformation Settings");
            ImGui::Text("Translation Speed");
            ImGui::SliderFloat("##2", &wContext.TRA_SPEED, 0.0f, 1.0f, "%.2f", flags);
            ImGui::Text("Rotation Speed");
            ImGui::SliderFloat("##3", &wContext.ROT_SPEED, 0.0f, 10.0f, "%.2f", flags);
            ImGui::Text("Scaling Speed");
            ImGui::SliderFloat("##4", &wContext.SCA_SPEED, 0.0f, 1.0f, "%.2f", flags);

            ImGui::End();
        }
    }

}