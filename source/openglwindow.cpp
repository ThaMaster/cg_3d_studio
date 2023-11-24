/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#include "openglwindow.h"

using namespace std;

// Initialize OpenGL context and viewport.
OpenGLWindow::OpenGLWindow(string title, int width, int height)
{
    // Initialize glfw
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Set minimum supported OpenGL version and OpenGL profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create OpenGL window
    windowWidth = width;
    windowHeight = height;
    glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (glfwWindow == nullptr) {
        glfwTerminate();
        cerr << "Could not open window or initialize OpenGL context." << endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1); 
    
    // Initialize glew
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        cout << "glew init error:" << endl << glewGetErrorString(glewError) << endl;
        exit(EXIT_FAILURE);
    }

    if ( !GLEW_VERSION_4_3 ) {
        cout << "Warning: OpenGL 4.3+ not supported by the GPU!" << endl;
        cout << "Decreace supported OpenGL version if needed." << endl;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init(NULL);

    // Set graphics attributes
    glPointSize(5.0); // Unsupported in OpenGL ES 2.0
    glLineWidth(1.0);
    glClearColor(0.2, 0.2, 0.2, 0.0);

    glViewport(0, 0, width, height);
}

OpenGLWindow::~OpenGLWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

// OpenGL error handler
bool OpenGLWindow::checkOpenGLError() const
{
    bool foundError = false;
    GLenum glError = glGetError();
    
    while (glError != GL_NO_ERROR) {
        cerr << "glError: " << glError << endl;
        foundError = true;
        glError = glGetError();
    }
    return foundError;
}

int OpenGLWindow::width() const
{
    return windowWidth;
}

int OpenGLWindow::height() const
{
    return windowHeight;
}

GLFWwindow* OpenGLWindow::window() const
{
    return glfwWindow;
}

float OpenGLWindow::getAspectRatio()
{
    return (float)windowWidth / (float) windowHeight;
}

// Read shader source files
string OpenGLWindow::readShaderSource(const string shaderFile) const
{
    string shaderSource;
    string line;

    fstream fs(shaderFile, ios::in);
    if(!fs)
        return shaderSource;

    while (!fs.eof()) {
        getline(fs, line);
        shaderSource.append(line + '\n');
    }
    fs.close();
    return shaderSource;
}


// Initialize OpenGL shader program
GLuint OpenGLWindow::initProgram(const string vShaderFile, const string fShaderFile) const
{
    GLuint program;
    int i;
    GLint  linked;

    struct shaders_t{
        string   filename;
        GLenum   type;
    };

    shaders_t shaders[2] = {
        { vShaderFile, GL_VERTEX_SHADER },
        { fShaderFile, GL_FRAGMENT_SHADER }
    };

    program = glCreateProgram();
    for (i = 0; i < 2; ++i ) {
        GLuint shader;
        GLint  compiled;

        string shaderSource = readShaderSource( shaders[i].filename );
        if ( shaderSource.empty() ) {
            cerr << "Failed to read " << shaders[i].filename << endl;
            exit( EXIT_FAILURE );
        }

        shader = glCreateShader( shaders[i].type );
        const char *shaderSrc = shaderSource.c_str();
        glShaderSource( shader, 1, &shaderSrc, NULL );
        glCompileShader( shader );
        checkOpenGLError();

        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLint  logSize;

            cerr << "Failed to compile " << shaders[i].filename << endl;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
            if (logSize > 0) {
                char logMsg[logSize+1];
                glGetShaderInfoLog( shader, logSize, nullptr, &(logMsg[0]) );
                cerr << "Shader info log: " << logMsg << endl;
            }
            exit( EXIT_FAILURE );
        }
        glAttachShader( program, shader );
    }

    /* link  and error check */
    glLinkProgram(program);
    checkOpenGLError();

    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
        GLint  logSize;

        cerr << "Shader program failed to link!" << endl;

        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
        if ( logSize > 0 ) {
            char logMsg[logSize+1];
            glGetProgramInfoLog( program, logSize, NULL, &(logMsg[0]) );
            cerr << "Program info log: " << logMsg << endl;
        }
        exit( EXIT_FAILURE );
    }

    return program;
}

// The window resize callback function
void OpenGLWindow::resizeCallback(GLFWwindow* window, int width, int height)
{
    reshape(width, height);
}

void OpenGLWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) 
    {
        // Detect which key has been pressed.
        switch(key)
        {
            // Detect Camera keys.
            case GLFW_KEY_A: cInfo.camOffset[0] = -CAM_SPEED; break;
            case GLFW_KEY_D: cInfo.camOffset[0] = CAM_SPEED; break;
            case GLFW_KEY_Q: cInfo.camOffset[1] = CAM_SPEED; break;
            case GLFW_KEY_E: cInfo.camOffset[1] = -CAM_SPEED; break;
            case GLFW_KEY_W: cInfo.camOffset[2] = -CAM_SPEED; break;
            case GLFW_KEY_S: cInfo.camOffset[2] = CAM_SPEED; break;
            // Detect Rotation keys.
            case GLFW_KEY_UP: tInfo.rVals[0] = -ROT_SPEED; break;
            case GLFW_KEY_DOWN: tInfo.rVals[0] = ROT_SPEED; break; 
            case GLFW_KEY_LEFT: tInfo.rVals[1] = -ROT_SPEED; break;
            case GLFW_KEY_RIGHT: tInfo.rVals[1] = ROT_SPEED; break;
            case GLFW_KEY_COMMA: tInfo.rVals[2] = ROT_SPEED; break;
            case GLFW_KEY_PERIOD: tInfo.rVals[2] = -ROT_SPEED; break;
            // Detect Translation keys.
            case GLFW_KEY_J: tInfo.tVals[0] = -TRA_SPEED; break;
            case GLFW_KEY_L: tInfo.tVals[0] = TRA_SPEED; break;
            case GLFW_KEY_I: tInfo.tVals[1] = TRA_SPEED; break;
            case GLFW_KEY_K: tInfo.tVals[1] = -TRA_SPEED; break; 
            case GLFW_KEY_Y: tInfo.tVals[2] = TRA_SPEED; break;
            case GLFW_KEY_H: tInfo.tVals[2] = -TRA_SPEED; break;
            // Detect Scaling keys.
            case GLFW_KEY_KP_ADD: tInfo.scVal = SCA_INC_SPEED; break;
            case GLFW_KEY_KP_SUBTRACT: tInfo.scVal = SCA_DEC_SPEED; break;
            // Detect reset key.
            case GLFW_KEY_R: tInfo.reset= true; break;
        } 
    } else if (action == GLFW_RELEASE) {
        // Reset values if key is released.
        if(key == GLFW_KEY_A || key == GLFW_KEY_D) cInfo.camOffset[0] = 0.0f;
        if(key == GLFW_KEY_Q || key == GLFW_KEY_E) cInfo.camOffset[1] = 0.0f;
        if(key == GLFW_KEY_W || key == GLFW_KEY_S) cInfo.camOffset[2] = 0.0f;
        if(key == GLFW_KEY_J || key == GLFW_KEY_L) tInfo.tVals[0] = 0.0f;
        if(key == GLFW_KEY_K || key == GLFW_KEY_I) tInfo.tVals[1] = 0.0f;
        if(key == GLFW_KEY_Y || key == GLFW_KEY_H) tInfo.tVals[2] = 0.0f;
        if(key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) tInfo.rVals[0] = 0.0f;
        if(key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) tInfo.rVals[1] = 0.0f;
        if(key == GLFW_KEY_COMMA || key == GLFW_KEY_PERIOD) tInfo.rVals[2] = 0.0f;
        if(key == GLFW_KEY_KP_ADD || key == GLFW_KEY_KP_SUBTRACT) tInfo.scVal = 0.0f;
    }
}

// GLFW error callback function
void OpenGLWindow::errorCallback(int error, const char* description)
{
    cerr << "GLFW error: " << description << endl;
}

// Start the GLFW loop
void OpenGLWindow::start()
{
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(glfwWindow)) {
        glfwPollEvents();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui example gui
        //ImGui::ShowDemoWindow(&show_demo_window);

        // Draw the gui
        DrawGui();

        updateObject();

        // Call display in geomentryRender to render the scene
        display();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Swap buffers
        glfwSwapBuffers(glfwWindow);

        // Sleep and wait for an event
        //glfwWaitEvents();
    }
    
}

// Render the scene 
void OpenGLWindow::displayNow()
{
    if (glfwGetCurrentContext() == nullptr)
        return;

    display();
}

/**
 * Reshape the GLFW window if the window is resized.
 * 
 * @param width The new width to reshape the window to.
 * @param height The new height to reshape the window to.
 * 
 */
void OpenGLWindow::reshape(const int width, const int height) const
{
    if (glfwGetCurrentContext() == nullptr) 
        return;
    glViewport(-1,-1, width, height);
}

void OpenGLWindow::DrawGui()
{
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context.");
    
    if(showObjTransWindow) objTransWindow();
    if(showObjInfWindow) objInfWindow();
    if(showCamWindow) camWindow();
    
    if(showKeyRefWindow) keyRefWindow();

    mainMenuBar();

    handleMouseInput();

    if(showOverlay) showStudioOverlay(&showOverlay);
}

void OpenGLWindow::handleMouseInput() 
{
    if(ImGui::IsMouseDragging(ImGuiMouseButton_Left, -1.0f) && !ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow)) {
        cInfo.camRotOffset = glm::vec3(ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y, 0.0f);
    } else {
        cInfo.camRotOffset = glm::vec3(0.0f,0.0f,0.0f);
    }
}

void OpenGLWindow::showStudioOverlay(bool *open) 
{
    char fileName[objFileName.length()+1];
    strcpy(fileName, objFileName.c_str());
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
    if(ImGui::Begin("Overlay", open, window_flags))
    {
        ImGui::SeparatorText("3D Studio v0.0.1");
        if(oInfo.objectLoaded) ImGui::Text("Loaded object: %s", fileName);

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Top-left (default)",     NULL, location == 0)) location = 0;
            if (ImGui::MenuItem("Top-right",    NULL, location == 1)) location = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, location == 2)) location = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
            if (open && ImGui::MenuItem("Close")) *open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void OpenGLWindow::mainMenuBar()
{    
    static ImGuiFileDialog fileDialog;

    if (ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Open")) fileDialog.OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", ".");  
            if(ImGui::MenuItem("Settings")) {} 
            ImGui::Separator();         
            if(ImGui::MenuItem("Quit")) { glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE); }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View"))
        {
            ImGui::BeginDisabled(!oInfo.objectLoaded);
            if(ImGui::MenuItem("Object Transformation", "F1", showObjTransWindow)) { showObjTransWindow = !showObjTransWindow; }
            if(ImGui::MenuItem("Object Information", "F2", showObjInfWindow)) { showObjInfWindow = !showObjInfWindow; }
            ImGui::Separator();  
            if(ImGui::MenuItem("Camera", "F3", showCamWindow)) { showCamWindow = !showCamWindow; }
            ImGui::EndDisabled();
            ImGui::Separator();
            if(ImGui::MenuItem("Studio Overlay", NULL, showOverlay)) { showOverlay = !showOverlay; }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Help")) 
        {
            if(ImGui::MenuItem("Introduction")) {}
            if(ImGui::MenuItem("Keyboard Shortcuts Reference", "F9", showKeyRefWindow)) { showKeyRefWindow = !showKeyRefWindow; }
            ImGui::Separator();   
            if(ImGui::MenuItem("About")) aboutOpen = true;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (fileDialog.Display("ChooseFileDlgKey")) {
        if (fileDialog.IsOk() == true) {
            objFileName = fileDialog.GetCurrentFileName();
            objFilePath = fileDialog.GetCurrentPath();
            loadObjectFromGui(objFileName);
        }
        fileDialog.Close();
    }

    aboutPopupModal();
}

// Contains only PLACEHOLDER for now.
void OpenGLWindow::aboutPopupModal() {
    if(aboutOpen) {
        ImGui::OpenPopup("About");    
    }
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

// Contains only PLACEHOLDER for now.
void OpenGLWindow::objTransWindow()
{
    ImGui::Begin("Object Transformation", &showObjTransWindow, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SeparatorText("Translation");
    ImGui::Text("--placeholder--");
    ImGui::SeparatorText("Scaling");
    ImGui::Text("--placeholder--");
    ImGui::SeparatorText("Rotation");
    ImGui::Text("--placeholder--");
    ImGui::SeparatorText("Shearing");
    ImGui::Text("--placeholder--");
    ImGui::SeparatorText("Reset");
    if(ImGui::Button("Reset Object Transformations")) { tInfo.reset = true; }
    ImGui::End();
}

void OpenGLWindow::objInfWindow()
{
    char fileName[objFileName.length()+1];
    char filePath[objFilePath.length()+objFileName.length()+2];
    strcpy(fileName, objFileName.c_str());
    strcpy(filePath, (objFilePath+"/"+objFileName).c_str());
    ImGui::Begin("Object Information", &showObjInfWindow, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SeparatorText("Object Information");
    ImGui::Text("Object File Name: %s", fileName);
    ImGui::Text("File Path: %s", filePath);
    ImGui::Separator();
    ImGui::Text("Shapes:");
    ImGui::SameLine(200); ImGui::Text("%d", oInfo.nShapes);
    ImGui::Text("Vertices:");
    ImGui::SameLine(200); ImGui::Text("%d", oInfo.nVertices);
    ImGui::Text("Indices:");
    ImGui::SameLine(200); ImGui::Text("%d", oInfo.nIndices);
    ImGui::Text("Faces:");
    ImGui::SameLine(200); ImGui::Text("%d", oInfo.nFaces);
    ImGui::Text("Normals:");
    ImGui::SameLine(200); ImGui::Text("%d", oInfo.nNormals);
    ImGui::Text("Texture Coordinates:");
    ImGui::SameLine(200); ImGui::Text("%d", oInfo.nTexCoords);
    ImGui::Text("Color Values:");
    ImGui::SameLine(200); ImGui::Text("%d", oInfo.nColors);
    ImGui::End();
}

void OpenGLWindow::camWindow()
{
    static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;

    ImGui::Begin("Camera", &showCamWindow, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SeparatorText("View");
    ImGui::Text("Camera Position (pZero): ");
    ImGui::PushItemWidth(100);
    ImGui::SliderFloat("X##zero", &cInfo.pZero.x, -50.0f, 50.0f); ImGui::SameLine(); 
    ImGui::SliderFloat("Y##zero", &cInfo.pZero.y, -50.0f, 50.0f); ImGui::SameLine(); 
    ImGui::SliderFloat("Z##zero", &cInfo.pZero.z, -50.0f, 50.0f);
    ImGui::PopItemWidth();
    if(ImGui::Button("Reset Camera Position")) { cInfo.pZero = pZeroDefault; }
    ImGui::Text("Reference Point (pRef): ");
    ImGui::PushItemWidth(100);
    ImGui::SliderFloat("X##ref", &cInfo.pRef.x, -500.0f, 500.0f); ImGui::SameLine();
    ImGui::SliderFloat("Y##ref", &cInfo.pRef.y, -500.0f, 500.0f); ImGui::SameLine();
    ImGui::SliderFloat("Z##ref", &cInfo.pRef.z, -500.0f, 500.0f);
    ImGui::PopItemWidth();
    if(ImGui::Button("Reset Reference Point")) { cInfo.pRef = pRefDefault; }
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

// Contains only PLACEHOLDER for now.
void OpenGLWindow::keyRefWindow()
{
    ImGui::Begin("Keyboard Shortcut Reference", &showKeyRefWindow, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SeparatorText("Shortcuts");
    ImGui::Text("--placeholder");
    ImGui::End();
}
