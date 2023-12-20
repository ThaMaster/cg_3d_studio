/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#include "openglwindow.h"
#include "studiogui.h"
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
    static ImGuiFileDialog fileDialog;
    
    if(action == GLFW_PRESS) 
    {
        // Detect which key has been pressed.
        switch(key)
        {
            // Detect Camera keys.
            case GLFW_KEY_A: wContext.cInfo.camOffset[0] = -CAM_SPEED; break;
            case GLFW_KEY_D: wContext.cInfo.camOffset[0] = CAM_SPEED; break;
            case GLFW_KEY_Q: wContext.cInfo.camOffset[1] = CAM_SPEED; break;
            case GLFW_KEY_E: wContext.cInfo.camOffset[1] = -CAM_SPEED; break;
            case GLFW_KEY_W: wContext.cInfo.camOffset[2] = -CAM_SPEED; break;
            case GLFW_KEY_S: wContext.cInfo.camOffset[2] = CAM_SPEED; break;
            // Detect Rotation keys.
            case GLFW_KEY_UP: wContext.tInfo.rVals[0] = -ROT_SPEED; break;
            case GLFW_KEY_DOWN: wContext.tInfo.rVals[0] = ROT_SPEED; break; 
            case GLFW_KEY_LEFT: wContext.tInfo.rVals[1] = -ROT_SPEED; break;
            case GLFW_KEY_RIGHT: wContext.tInfo.rVals[1] = ROT_SPEED; break;
            case GLFW_KEY_COMMA: wContext.tInfo.rVals[2] = ROT_SPEED; break;
            case GLFW_KEY_PERIOD: wContext.tInfo.rVals[2] = -ROT_SPEED; break;
            // Detect Translation keys.
            case GLFW_KEY_J: wContext.tInfo.tVals[0] = -TRA_SPEED; break;
            case GLFW_KEY_L: wContext.tInfo.tVals[0] = TRA_SPEED; break;
            case GLFW_KEY_I: wContext.tInfo.tVals[1] = TRA_SPEED; break;
            case GLFW_KEY_K: wContext.tInfo.tVals[1] = -TRA_SPEED; break; 
            case GLFW_KEY_Y: wContext.tInfo.tVals[2] = TRA_SPEED; break;
            case GLFW_KEY_H: wContext.tInfo.tVals[2] = -TRA_SPEED; break;
            // Detect Scaling keys.
            case GLFW_KEY_KP_ADD: wContext.tInfo.scVal = SCA_INC_SPEED; break;
            case GLFW_KEY_KP_SUBTRACT: wContext.tInfo.scVal = SCA_DEC_SPEED; break;
            // Detect reset key.
            case GLFW_KEY_R: wContext.tInfo.reset= true; break;
            // Detect shortcut keys.
            case GLFW_KEY_O: /**/ break;
            case GLFW_KEY_F1: if(wContext.objects.size() != 0) wInfo.showObjTransWindow = !wInfo.showObjTransWindow; break;
            case GLFW_KEY_F2: if(wContext.objects.size() != 0) wInfo.showObjMatWindow = !wInfo.showObjMatWindow; break;
            case GLFW_KEY_F3: if(wContext.objects.size() != 0) wInfo.showObjInfWindow = !wInfo.showObjInfWindow; break;
            case GLFW_KEY_F4: wInfo.showCamWindow = !wInfo.showCamWindow; break;
            case GLFW_KEY_F5: wInfo.showLightSourcesWindow = !wInfo.showLightSourcesWindow; break;
            case GLFW_KEY_F9: wInfo.showKeyRefWindow = !wInfo.showKeyRefWindow; break;
            case GLFW_KEY_F10: wInfo.showLogWindow = !wInfo.showLogWindow; break;
        } 
    } else if (action == GLFW_RELEASE) {
        // Reset values if key is released.
        if(key == GLFW_KEY_A || key == GLFW_KEY_D) wContext.cInfo.camOffset[0] = 0.0f;
        if(key == GLFW_KEY_Q || key == GLFW_KEY_E) wContext.cInfo.camOffset[1] = 0.0f;
        if(key == GLFW_KEY_W || key == GLFW_KEY_S) wContext.cInfo.camOffset[2] = 0.0f;
        if(key == GLFW_KEY_J || key == GLFW_KEY_L) wContext.tInfo.tVals[0] = 0.0f;
        if(key == GLFW_KEY_K || key == GLFW_KEY_I) wContext.tInfo.tVals[1] = 0.0f;
        if(key == GLFW_KEY_Y || key == GLFW_KEY_H) wContext.tInfo.tVals[2] = 0.0f;
        if(key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) wContext.tInfo.rVals[0] = 0.0f;
        if(key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) wContext.tInfo.rVals[1] = 0.0f;
        if(key == GLFW_KEY_COMMA || key == GLFW_KEY_PERIOD) wContext.tInfo.rVals[2] = 0.0f;
        if(key == GLFW_KEY_KP_ADD || key == GLFW_KEY_KP_SUBTRACT) wContext.tInfo.scVal = 0.0f;
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
        handleMouseInput();
        // Draw the gui
        DrawGui();

        updateObject();
        updateCamera();
        updateLight();
        updateMaterial();


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
    StudioGui::mainMenuBar(glfwWindow, wInfo, wContext);
    if(wContext.objects.size() != 0) {
        StudioGui::objTransWindow(wInfo.showObjTransWindow, wContext.tInfo.reset);
        StudioGui::objMatWindow(wInfo.showObjMatWindow, wContext.objects[0]);
        StudioGui::objInfWindow(wInfo.showObjInfWindow, objFileName, objFilePath, wContext.objects[0].oInfo);
    }
    StudioGui::camWindow(wInfo.showCamWindow, wContext.cInfo, wContext.pZeroDefault, pRefDefault);
    StudioGui::showLightSourcesWindow(wInfo.showLightSourcesWindow, wContext.light);
    StudioGui::keyRefWindow(wInfo.showKeyRefWindow);
    if(wContext.objects.size() != 0) {
        StudioGui::showStudioOverlay(wInfo.showOverlay, objFileName, wContext.objects[0].oInfo);
    } else {
        StudioGui::showEmptyStudioOverlay(wInfo.showOverlay);

    }

    if(wInfo.openFileDialog) openFile();

    StudioGui::logWindow(wInfo.showLogWindow, log);
}

void OpenGLWindow::openFile()
{
    static ImGuiFileDialog fileDialog;
    fileDialog.OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", ".");
    if (fileDialog.Display("ChooseFileDlgKey")) {
        if (fileDialog.IsOk() == true) {
            objFileName = fileDialog.GetCurrentFileName();
            objFilePath = fileDialog.GetCurrentPath();
            loadObjectFromGui(objFileName);
        }
        fileDialog.Close();
        wInfo.openFileDialog = false;
    }
}

void OpenGLWindow::handleMouseInput() 
{
    if(ImGui::IsMouseDragging(ImGuiMouseButton_Left, -1.0f) && !ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow)) {
        wContext.cInfo.camRotOffset = glm::vec3(-ImGui::GetIO().MouseDelta.x, -ImGui::GetIO().MouseDelta.y, 0.0f);
    } else {
        wContext.cInfo.camRotOffset = glm::vec3(0.0f,0.0f,0.0f);
    }
}
