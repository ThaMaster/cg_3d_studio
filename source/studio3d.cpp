#include "studio3d.h"
#include "studiogui.h"

using namespace std;

/**
 * This is the main class of the whole program. This class
 * compunicates with many classes in order for loadning new
 * objects to the scene, rendering these objects, transforming
 * these objects and more. This is essentially the center
 * of the program.
 * 
 * Author: Christoffer Nordlnader (c20cnr@cs.umu.se)
 * 
 * Version information:
 *      2024-01-08: v1.0, first version.
 */

/**
 * Constructor of the 3D studio. Initializes the OpenGL
 * context and the viewport that will be used when
 * rendering in the scene.
 * 
 * @param title: The title of the window.
 * @param width: The starting width of the window.
 * @param height: The starting height of the window.
 */
Studio3D::Studio3D(string title, int width, int height)
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init(NULL);

    // Set graphics attributes
    glPointSize(5.0);
    glLineWidth(1.0);
    glClearColor(0.2, 0.2, 0.2, 0.0);

    glViewport(0, 0, width, height);
}

/**
 * Deconstructor of the class. Shutsdown all the
 * GLFW instances and destroys the window.
 */
Studio3D::~Studio3D()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

/**
 * Function for checking if any errors have occurred
 * during reading the shader files.
 * 
 * @return: True/False if any errors has occurred.
 */
bool Studio3D::checkOpenGLError() const
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

/**
 * Function for getting the window width of the program.
 * 
 * @return The width of the window of the program.
 */
int Studio3D::width() const
{
    return windowWidth;
}

/**
 * Function for getting the window height of the program.
 * 
 * @return The height of the window of the program.
 */
int Studio3D::height() const
{
    return windowHeight;
}

/**
 * Function for getting the window of the program.
 * 
 * @return Pointer to the window of the program.
 */
GLFWwindow* Studio3D::window() const
{
    return glfwWindow;
}

/**
 * Function for getting the aspect ratio of the window.
 * 
 * @return The aspect ratio.
 */
float Studio3D::getAspectRatio()
{
    return (float)windowWidth / (float) windowHeight;
}

/**
 * Function for reading the specified shader file. 
 * 
 * @param shaderFile: The file path to the shader.
 * 
 * @return The contents of the shader file.
 */
string Studio3D::readShaderSource(const string shaderFile) const
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

/**
 * Initialize the specified vertex and fragment shader to 
 * the program. If any errors ocurr, the content will be
 * directed to standard output since the program will not
 * be able to start if any errors were to occur.
 * 
 * @param vShaderFile: The file path to the vertex shader code.
 * @param fShaderFile: The file path to the fragment shader code.
 */ 
GLuint Studio3D::initProgram(const string vShaderFile, const string fShaderFile) const
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

/**
 * The window resize callback function. Reshapes the window
 * to the new width and height.
 * 
 * @param window: The window to be resized.
 * @param width: The new width of the window.
 * @param height: The new height of the window.
 */
void Studio3D::resizeCallback(GLFWwindow* window, int width, int height)
{
    reshape(width, height);
}

/**
 * Function for handling all the keyboard input. When a key has been
 * pressed it will perform its corresponding action. When the key is later
 * released, it will reset all the values that are necessary.
 * 
 * @param window: The window of the program.
 * @param key: The key that caused the callback.
 * @param scancode: The scancode of the key that caused the callback.
 * @param action: The action which were performed on the key.
 * @param mods: The input mode.
 */
void Studio3D::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static ImGuiFileDialog fileDialog;
    
    if(action == GLFW_PRESS) 
    {
        // Detect which key has been pressed.
        switch(key)
        {
            // Detect Camera keys.
            case GLFW_KEY_A: wContext.cInfo.camOffset[0] = -wContext.CAM_SPEED; break;
            case GLFW_KEY_D: wContext.cInfo.camOffset[0] = wContext.CAM_SPEED; break;
            case GLFW_KEY_Q: wContext.cInfo.camOffset[1] = wContext.CAM_SPEED; break;
            case GLFW_KEY_E: wContext.cInfo.camOffset[1] = -wContext.CAM_SPEED; break;
            case GLFW_KEY_W: wContext.cInfo.camOffset[2] = -wContext.CAM_SPEED; break;
            case GLFW_KEY_S: wContext.cInfo.camOffset[2] = wContext.CAM_SPEED; break;
            // Detect Rotation keys.
            case GLFW_KEY_UP: wContext.tInfo.rVals[0] = -wContext.ROT_SPEED; break;
            case GLFW_KEY_DOWN: wContext.tInfo.rVals[0] = wContext.ROT_SPEED; break; 
            case GLFW_KEY_LEFT: wContext.tInfo.rVals[1] = -wContext.ROT_SPEED; break;
            case GLFW_KEY_RIGHT: wContext.tInfo.rVals[1] = wContext.ROT_SPEED; break;
            case GLFW_KEY_COMMA: wContext.tInfo.rVals[2] = wContext.ROT_SPEED; break;
            case GLFW_KEY_PERIOD: wContext.tInfo.rVals[2] = -wContext.ROT_SPEED; break;
            // Detect Translation keys.
            case GLFW_KEY_J: wContext.tInfo.tVals[0] = -wContext.TRA_SPEED; break;
            case GLFW_KEY_L: wContext.tInfo.tVals[0] = wContext.TRA_SPEED; break;
            case GLFW_KEY_I: wContext.tInfo.tVals[1] = wContext.TRA_SPEED; break;
            case GLFW_KEY_K: wContext.tInfo.tVals[1] = -wContext.TRA_SPEED; break; 
            case GLFW_KEY_Y: wContext.tInfo.tVals[2] = wContext.TRA_SPEED; break;
            case GLFW_KEY_H: wContext.tInfo.tVals[2] = -wContext.TRA_SPEED; break;
            // Detect Scaling keys.
            case GLFW_KEY_KP_ADD: wContext.tInfo.scVal = 1+wContext.SCA_SPEED; break;
            case GLFW_KEY_KP_SUBTRACT: wContext.tInfo.scVal = 1-wContext.SCA_SPEED; break;
            // Detect reset key.
            case GLFW_KEY_R: wContext.tInfo.reset= true; break;
            // Detect shortcut keys.
            case GLFW_KEY_O: /**/ break;
            case GLFW_KEY_F1:  wInfo.showSceneWindow = !wInfo.showSceneWindow; break;
            case GLFW_KEY_F3: if(wContext.objects.size() != 0) wInfo.showObjMatWindow = !wInfo.showObjMatWindow; break;
            case GLFW_KEY_F4: if(wContext.objects.size() != 0) wInfo.showObjInfWindow = !wInfo.showObjInfWindow; break;
            case GLFW_KEY_F5: wInfo.showCamWindow = !wInfo.showCamWindow; break;
            case GLFW_KEY_F6: wInfo.showLightSourcesWindow = !wInfo.showLightSourcesWindow; break;
            case GLFW_KEY_F9: wInfo.showLogWindow = !wInfo.showLogWindow; break;
            case GLFW_KEY_F10: wInfo.showKeyRefWindow = !wInfo.showKeyRefWindow; break;
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
void Studio3D::errorCallback(int error, const char* description)
{
    cerr << "GLFW error: " << description << endl;
}

/**
 * Function for starting the rendering loop where
 * all the rendering and user interaction are occuring.
 * If the window is emitting that it should close, the 
 * loop will be stopped and the program will be exited.
 */
void Studio3D::start()
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

        updateObject(wContext.selectedObject);
        updateCamera();
        updateLight();
        
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

/**
 * Reshape the GLFW window if the window is resized.
 * 
 * @param width The new width to reshape the window to.
 * @param height The new height to reshape the window to.
 * 
 */
void Studio3D::reshape(const int width, const int height) const
{
    if (glfwGetCurrentContext() == nullptr) 
        return;
    glViewport(-1,-1, width, height);
}

/**
 * Function for drawing the entire graphical user interface. It
 * will use the window information and world context to get
 * all the information needed in order to show the interface 
 * correctly.
 */
void Studio3D::DrawGui()
{
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context.");
    StudioGui::mainMenuBar(glfwWindow, wInfo, wContext);
    StudioGui::sceneWindow(wInfo.showSceneWindow, wContext);

    if(wContext.objects.size() != 0) {
        StudioGui::objMatWindow(wInfo.showObjMatWindow, wContext.objects[wContext.selectedObject]);
        StudioGui::objInfWindow(wInfo.showObjInfWindow, wContext.objects[wContext.selectedObject].fileName, wContext.objects[wContext.selectedObject].oInfo);
    }
    StudioGui::camWindow(wInfo.showCamWindow, wContext.cInfo, wContext.pZeroDefault, wContext.pRefDefault);
    StudioGui::showLightSourcesWindow(wInfo.showLightSourcesWindow, wContext);
    StudioGui::keyRefWindow(wInfo.showKeyRefWindow);
    StudioGui::showStudioOverlay(wInfo.showOverlay, wContext);

    if(wInfo.openObjFileDialog) openObjectFile();
    if(wInfo.openTexFileDialog) openTextureFile();

    StudioGui::settingsWindow(wInfo.showSettingsWindow, wContext);
    StudioGui::logWindow(wInfo.showLogWindow, log);
}

/**
 * Function for creating a FileDialog window in order
 * to load an object file. The FileDialog will only show
 * ".obj" files since these are the only supported for this
 * program. Any output created when attemptning to load the
 * file will be added to the logger.
 */
void Studio3D::openObjectFile()
{
    static ImGuiFileDialog objFileDialog;
    std::string loaderOutput;
    objFileDialog.OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", ".");
    if (objFileDialog.Display("ChooseFileDlgKey")) {
        if (objFileDialog.IsOk() == true) {
            objFileName = objFileDialog.GetCurrentFileName();
            objFilePath = objFileDialog.GetCurrentPath();
            loaderOutput = loadObjectFromGui(objFilePath, objFileName);
        }
        objFileDialog.Close();
        wInfo.openObjFileDialog = false;
    }
    log.addLog(loaderOutput.c_str());
}

/**
 * Function for creating a FileDialog window in order
 * to load a texture file on the currently selected
 * object. The FileDialog will only show ".jpg" files
 * but can be changed in the window. Any ouput created
 * when attempting to load the texture file will be 
 * added to the logger object.
 */
void Studio3D::openTextureFile()
{
    static ImGuiFileDialog texFileDialog;
    std::string textureOutput;
    texFileDialog.OpenDialog("ChooseFileDlgKey", "Choose File", ".jpg", ".");
    if (texFileDialog.Display("ChooseFileDlgKey")) {
        if (texFileDialog.IsOk() == true) {
            texName = texFileDialog.GetCurrentFileName();
            texPath = texFileDialog.GetCurrentPath();
            textureOutput = loadTextureFromGui(texName, texPath, wContext.selectedObject);
        }
        texFileDialog.Close();
        wInfo.openTexFileDialog = false;
    }
    log.addLog(textureOutput.c_str());
}

/**
 * Function for handling the mouse input. Depending on the direction the
 * mouse is moving changes the camera rotation offset ONLY if the 
 * left mouse button is pressed.
 */
void Studio3D::handleMouseInput() 
{
    if(ImGui::IsMouseDragging(ImGuiMouseButton_Left, -1.0f) && !ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow)) {
        wContext.cInfo.camRotOffset = glm::vec3(-ImGui::GetIO().MouseDelta.x, -ImGui::GetIO().MouseDelta.y, 0.0f);
    } else {
        wContext.cInfo.camRotOffset = glm::vec3(0.0f,0.0f,0.0f);
    }
}
