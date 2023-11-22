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
bool 
OpenGLWindow::checkOpenGLError() const
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

int 
OpenGLWindow::width() const
{
    return windowWidth;
}

int 
OpenGLWindow::height() const
{
    return windowHeight;
}

GLFWwindow* 
OpenGLWindow::window() const
{
    return glfwWindow;
}

float
OpenGLWindow::getAspectRatio()
{
    return (float)windowWidth / (float) windowHeight;
}

// Read shader source files
string
OpenGLWindow::readShaderSource(const string shaderFile) const
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
GLuint 
OpenGLWindow::initProgram(const string vShaderFile, const string fShaderFile) const
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
void 
OpenGLWindow::resizeCallback(GLFWwindow* window, int width, int height)
{
    reshape(width, height);
}

void
OpenGLWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
            case GLFW_KEY_UP: oInfo.rVals[0] = -ROT_SPEED; break;
            case GLFW_KEY_DOWN: oInfo.rVals[0] = ROT_SPEED; break; 
            case GLFW_KEY_LEFT: oInfo.rVals[1] = -ROT_SPEED; break;
            case GLFW_KEY_RIGHT: oInfo.rVals[1] = ROT_SPEED; break;
            case GLFW_KEY_COMMA: oInfo.rVals[2] = ROT_SPEED; break;
            case GLFW_KEY_PERIOD: oInfo.rVals[2] = -ROT_SPEED; break;
            // Detect Translation keys.
            case GLFW_KEY_J: oInfo.tVals[0] = -TRA_SPEED; break;
            case GLFW_KEY_L: oInfo.tVals[0] = TRA_SPEED; break;
            case GLFW_KEY_I: oInfo.tVals[1] = TRA_SPEED; break;
            case GLFW_KEY_K: oInfo.tVals[1] = -TRA_SPEED; break; 
            case GLFW_KEY_Y: oInfo.tVals[2] = TRA_SPEED; break;
            case GLFW_KEY_H: oInfo.tVals[2] = -TRA_SPEED; break;
            // Detect Scaling keys.
            case GLFW_KEY_KP_ADD: oInfo.scVal = SCA_INC_SPEED; break;
            case GLFW_KEY_KP_SUBTRACT: oInfo.scVal = SCA_DEC_SPEED; break;
            // Detect reset key.
            case GLFW_KEY_R: oInfo.reset= true; break;
        } 
    } else if (action == GLFW_RELEASE) {
        // Reset values if key is released.
        if(key == GLFW_KEY_A || key == GLFW_KEY_D) cInfo.camOffset[0] = 0.0f;
        if(key == GLFW_KEY_Q || key == GLFW_KEY_E) cInfo.camOffset[1] = 0.0f;
        if(key == GLFW_KEY_W || key == GLFW_KEY_S) cInfo.camOffset[2] = 0.0f;
        if(key == GLFW_KEY_J || key == GLFW_KEY_L) oInfo.tVals[0] = 0.0f;
        if(key == GLFW_KEY_K || key == GLFW_KEY_I) oInfo.tVals[1] = 0.0f;
        if(key == GLFW_KEY_Y || key == GLFW_KEY_H) oInfo.tVals[2] = 0.0f;
        if(key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) oInfo.rVals[0] = 0.0f;
        if(key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) oInfo.rVals[1] = 0.0f;
        if(key == GLFW_KEY_COMMA || key == GLFW_KEY_PERIOD) oInfo.rVals[2] = 0.0f;
        if(key == GLFW_KEY_KP_ADD || key == GLFW_KEY_KP_SUBTRACT) oInfo.scVal = 0.0f;
        if(key == GLFW_KEY_R) oInfo.reset = false;
    }
}

// GLFW error callback function
void 
OpenGLWindow::errorCallback(int error, const char* description)
{
    cerr << "GLFW error: " << description << endl;
}

// Start the GLFW loop
void 
OpenGLWindow::start()
{
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(glfwWindow)) {
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
        glfwWaitEvents();
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

void
OpenGLWindow::DrawGui()
{
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context.");
    
    if(showObjTransWindow) objTransWindow();
    if(showObjInfWindow) objInfWindow();
    if(showCamWindow) camWindow();

    mainMenuBar();

    handleMouseInput();

    if(showOverlay) showStudioOverlay(&showOverlay);
}

void OpenGLWindow::handleMouseInput() {
    if(ImGui::IsMouseDragging(ImGuiMouseButton_Left, -1.0f) && !ImGui::IsWindowHovered(ImGuiFocusedFlags_AnyWindow)) {
        cInfo.camRotOffset += glm::vec3(ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y, 0.0f);
    }
}

void OpenGLWindow::showStudioOverlay(bool *open) 
{
    static int location = 0;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    const float PAD = 10.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
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
        ImGui::Text("3D studio overlay");
        ImGui::Separator();
        if (ImGui::BeginPopupContextWindow())
        {
            if (open && ImGui::MenuItem("Close")) *open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void OpenGLWindow::mainMenuBar()
{
    static string objFileName;
    static string objFilePath;
    
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
            if(ImGui::MenuItem("Object Transformation", "F1", showObjTransWindow)) { showObjTransWindow = !showObjTransWindow; }
            if(ImGui::MenuItem("Object Information", "F2", showObjInfWindow)) { showObjInfWindow = !showObjInfWindow; }
            ImGui::Separator();  
            if(ImGui::MenuItem("Camera", "F3", showCamWindow)) { showCamWindow = !showCamWindow; }
            ImGui::Separator();
            if(ImGui::MenuItem("Studio Overlay", NULL, showOverlay)) { showOverlay = !showOverlay; }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Help")) 
        {
            if(ImGui::MenuItem("Introduction")) {}
            if(ImGui::MenuItem("Keyboard Shortcuts Reference")) {}
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
    ImGui::End();
}

void OpenGLWindow::objInfWindow()
{
    ImGui::Begin("Object Information", &showObjInfWindow, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SeparatorText("Shape Info");
    ImGui::Text("--placeholder--");
    ImGui::SeparatorText("Vertex Info");
    ImGui::Text("--placeholder--");
    ImGui::SeparatorText("More...");
    ImGui::Text("--placeholder--");
    ImGui::End();
}

void OpenGLWindow::camWindow()
{
    static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;

    ImGui::Begin("Camera", &showCamWindow, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SeparatorText("Camera Position");
    ImGui::Text("--placeholder--");
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