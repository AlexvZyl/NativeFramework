/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

// General.
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <thread>

// ImGUI (GUI software). 
#include "Core/imgui.h"
#include "Implementations/imgui_impl_glfw.h"
#include "Implementations/imgui_impl_opengl3.h"
#include "stateMachine.h"

// GUI includes.
#include "GUI/guiHandler.h"

// Graphics handler include.
#include <../Graphics/graphicsHandler.h>

// Resources.
#include <Misc/stb_image.h>

// Include GLFW (window) after OpenGL definition.
#include <GLFW/glfw3.h>

// GLAD (OpenGL loader).
#include <glad/glad.h>

/*=======================================================================================================================================*/
/* Compiler settings.                                                                                                                    */
/*=======================================================================================================================================*/

// This removes the console that keeps opening with the app if it is not in debug mode.
#ifdef	_DEBUG
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
#else
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/*=======================================================================================================================================*/
/* Variables/Globals/Defines.                                                                                                            */
/*=======================================================================================================================================*/

// Defined here, assigned in the main function where it has access to the window.
// Used as a global variable so that the mouse event callbacks from GLFW can have
// access to it.
GraphicsHandler* graphicsHandler;

/*=======================================================================================================================================*/
/* Python interface.                                                                                                                     */
/*=======================================================================================================================================*/

// Hash the input text to be checked against list of commands.
constexpr size_t hash(const char* str) {
    const long long p = 131;
    const long long m = 4294967291; // 2^32 - 5, largest 32 bit prime
    long long total = 0;
    long long current_multiplier = 1;
    for (int i = 0; str[i] != '\0'; ++i) {
        total = (total + current_multiplier * str[i]) % m;
        current_multiplier = (current_multiplier * p) % m;
    }
    return total;
}

// Process the input from python.
// Divided into different types and processed accordingly.
void procesInput(std::string inString, stateMachine* states) {

    // Variables.
    int startFunc;
    int endFunc;

    // Functions is shown between (: and :)
    startFunc = (int)inString.find("[") + 1;
    endFunc = (int)inString.find("]", startFunc) - 1;

    // Parameters is shown between (: and :).
    std::string command = inString.substr(startFunc, endFunc);
    inString = inString.substr(endFunc + 2);

    // Parse into types.
    switch (hash(command.c_str()))
    {

        // Quit the thread.
        case hash("Quit"):
        {
            states->globalQuit = true;
            break;
        }
         
        // Command to be processed by the app.
        case hash("Command"):
        {
            std::string function = inString.substr(1, inString.find("]") - 1);
            inString = inString.substr(inString.find("]") + 1);
            std::string params = inString.substr(1, inString.find("]") - 1);
            states->inputQueueMCC.push(*(new inputQueue(function, params)));
            break;
        }

        default:
        {
            std::cout << "[INTERFACE][ERROR] '" << command << "' type invalid. \n\n";
            break;
        }
    }
}

// Threaded function that reads the messages from python.
void readingIn(stateMachine* states) {

    std::string temp;
    while (true) {

        std::getline(std::cin, temp);
        if (temp != "") {
            procesInput(temp, states);
        }
        temp = "";
    }
}

// Process the funcions laoded by the python thread.
void deQueueInput(stateMachine* states) {
    
    // Run while there are commands left to process.
    while (states->inputQueueMCC.size() > 0) {

        // Processing variables.
        inputQueue temp = states->inputQueueMCC.front();
        std::string mccName;
        std::string text;
        std::vector<float> params;

        // Switch between different commands.
        switch (hash(temp.command.c_str())) {

            // Draw line.
            case hash("drawLine"):
                try {
                    mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    for (size_t i = 0; i < 8; i++)
                    {
                        params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                        temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    }
                    graphicsHandler->m_mccEngine->drawLine(mccName, new float[2]{ params[0],params[1] }, new float[2]{ params[2],params[3] }, new float[4]{ params[4],params[5],params[6],params[7] });
                }
                catch (const std::exception& e)
                {
                    std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
                }
                break;

            // Draw clear triangle.
            case hash("drawTriangleClear"):
                try {
                    mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    for (size_t i = 0; i < 10; i++)
                    {
                        params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                        temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    }
                    graphicsHandler->m_mccEngine->drawTriangleClear(mccName, new float[2]{ params[0],params[1] }, new float[2]{ params[2],params[3] }, new float[2]{ params[4], params[5] }, new float[4]{ params[6],params[7],params[8],params[9] });
                }
                catch (const std::exception& e)
                {
                    std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
                }
                break;

            // Draw filled triangle. 
            case hash("drawTriangleFilled"):
                try {
                    mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    for (size_t i = 0; i < 10; i++)
                    {
                        params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                        temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    }
                    graphicsHandler->m_mccEngine->drawTriangleFilled(mccName, new float[2]{ params[0],params[1] }, new float[2]{ params[2],params[3] }, new float[2]{ params[4], params[5] }, new float[4]{ params[6],params[7],params[8],params[9] });
                }
                catch (const std::exception& e)
                {
                    std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
                }
                break;

            // Draw clear quad.
            case hash("drawQuadClear"):
                try {
                    mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    for (size_t i = 0; i < 8; i++)
                    {
                        params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                        temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    }
                    graphicsHandler->m_mccEngine->drawQuadClear(mccName, new float[2]{ params[0],params[1] }, params[2], params[3], new float[4]{ params[4],params[5],params[6],params[7] });
                }
                catch (const std::exception& e)
                {
                    std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
                }
                break;

            // Draw filled quad.
            case hash("drawQuadFilled"):
                try {
                    mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    for (size_t i = 0; i < 8; i++)
                    {
                        params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                        temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    }
                    graphicsHandler->m_mccEngine->drawQuadFilled(mccName, new float[2]{ params[0],params[1] }, params[2], params[3], new float[4]{ params[4],params[5],params[6],params[7] });
                }
                catch (const std::exception& e)
                {
                    std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
                }
                break;

            // Draw clear circle.
            case hash("drawCircleClear"):
                try {
                    mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    for (size_t i = 0; i < 7; i++)
                    {
                        params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                        temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    }
                    graphicsHandler->m_mccEngine->drawCircleClear(mccName, new float[2]{ params[0],params[1] }, params[2], new float[4]{ params[3],params[4],params[5],params[6] });
                }
                catch (const std::exception& e)
                {
                    std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
                }
                break;

            // Draw a filled circle.
            case hash("drawCircleFilled"):
                try {
                    mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    for (size_t i = 0; i < 7; i++)
                    {
                        params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                        temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    }
                    graphicsHandler->m_mccEngine->drawCircleFilled(mccName, new float[2]{ params[0],params[1] }, params[2], new float[4]{ params[3],params[4],params[5],params[6] });
                }
                catch (const std::exception& e)
                {
                    std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
                }
                break;

            // Draw text.
            case hash("drawText"):
                try {
                    mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    for (size_t i = 0; i < 8; i++)
                    {
                        // Read text.
                        if (i == 0)
                        {
                            text = temp.parameters.substr(0, temp.parameters.find(";"));
                            temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                        }
                        // Read parameters,
                        else
                        {
                            params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                            temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                        }
                    }
                    graphicsHandler->m_mccEngine->drawText(mccName, text, new float[2]{ params[0],params[1] }, new float[4]{ params[2],params[3],params[4], params[5] }, params[6]);
                }
                catch (const std::exception& e)
                {
                    std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
                }
                break;

            // Add MCC window to draw.
            case hash("addMCC"):
                try {
                    mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    graphicsHandler->m_mccEngine->addMCC(mccName);
                }
                catch (const std::exception& e)
                {
                    std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" <<  e.what() << "'.\n\n";
                }
                break;

            // Remove MCC window.
            case hash("removeMCC"):
                try {
                    mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    graphicsHandler->m_mccEngine->removeMCC(mccName);
                }
                catch (const std::exception& e)
                {
                    std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
                }
                break;

            default:
                std::cout << "[INTERFACE][ERROR] '" << temp.command.c_str() << "' function invalid. \n\n";
                break;
            }

        // Remove command from queue.
        states->inputQueueMCC.pop();
    }
}

/*=======================================================================================================================================*/
/* GLFW callbacks.                                                                                                                       */
/*=======================================================================================================================================*/

// GLFW error handler.
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "[GLFW ERROR] %d: %s\n", error, description);
}

/*=======================================================================================================================================*/
/* Mouse events callbacks.                                                                                                               */
/*=======================================================================================================================================*/

// Handle mouse press events from GLFW.
void mousePressEvent(GLFWwindow* window, int button, int action, int mods)
{
    graphicsHandler->mousePressEvent(window, button, action, mods);
}

// Handle mouse press events from GLFW.
void mouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{
    graphicsHandler->mouseMoveEvent(window, (float)xpos, (float)ypos);
}

// Handle mouse press scroll. from GLFW.
void mouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
    graphicsHandler->mouseScrollEvent(window, (float)xoffset, (float)yoffset);
}

/*=======================================================================================================================================*/
/* Main                                                                                                                                  */
/*=======================================================================================================================================*/

int main(int, char**)
{
    // Setup window.
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // OpenGL Version select.
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100.
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #elif defined(__APPLE__)
        // GL 3.2 + GLSL 150.
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only.
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac.
    #else
        // GL 3.0 + GLSL 130.
        const char* glsl_version = "#version 450";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only.
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only.
    #endif

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    /* GLFW setup.                                                                                                                       */
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    // Enable 16x MSAA.
    glfwWindowHint(GLFW_SAMPLES, 16);
    // Create GLFW window.
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ElecDev", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync.

    // Set window icon.
    GLFWimage icon;
    icon.pixels = stbi_load("Source\\Resources\\Icons\\circuitIcon.bmp", &icon.width, &icon.height, NULL, STBI_rgb_alpha);
    glfwSetWindowIcon(window, 1, &icon);

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // Initialize OpenGL loader.
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    #if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
        bool err = gl3wInit() != 0;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
        bool err = glewInit() != GLEW_OK;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
        bool err = gladLoadGL() == 0;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
        bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
        bool err = false;
        glbinding::Binding::initialize();
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
        bool err = false;
        glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
    #else
        bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
    #endif

    // OpenGL loader error handler.
    if (err)
    {
        fprintf(stderr, "[OPENGL][ERROR] Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Print OpenGL version.
    std::cout << "[OPENGL][INFO] Loaded OpenGL version: " << glGetString(GL_VERSION) << ".\n\n";

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // ImGUI & OpenGL setup. 
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;
    // Enable keyboard controls.
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup ImGui style.
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    glfwMakeContextCurrent(window);

    // Viewport.
    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);

    // Setup mouse callbacks.
    glfwSetMouseButtonCallback(window, mousePressEvent);    // Mouse press event.
    glfwSetCursorPosCallback(window, mouseMoveEvent);       // Mouse move event.
    glfwSetScrollCallback(window, mouseScrollEvent);        // Mouse scroll event.

    // Enable MSAA in OpenGL.
    glEnable(GL_MULTISAMPLE);

    // Create the state machine variables.
    stateMachine* states = new stateMachine();
    states->toolsExpanded = false;
    states->toolsMode = 0;

    // Create graphics handler object.
    // For now a global variable is used to be able to have mouse callbacks with a method.
    // The callbacks cannot be used with a method, so it has to call a normal function.
    graphicsHandler = new GraphicsHandler(window, states);

    // Create GUI handler object.
    GUIHandler guiHandler(states, graphicsHandler);

    /*===================================================================================================================================*/
    /* Loop                                                                                                                              */
    /*===================================================================================================================================*/

    // Loop variables.
    bool wait = false;          // Wait for events.
    int display_w, display_h;   // Viewport for ImGUI.

    // Thread reading inputs from pipeline.
    std::thread t1(readingIn, states);

    // [MAIN LOOP] Graphics Pipeline
    while (!glfwWindowShouldClose(window) && !states->globalQuit)
    {
        // Poll commands from python interface.
        deQueueInput(states);

        // Event checking.
        if (wait) { glfwWaitEvents(); }   // App only runs when events occur.
        else { glfwPollEvents(); }        // App runs continuously.
    
        // Init colors for OpenGL.
        glClear(GL_COLOR_BUFFER_BIT);
     
        // Handle graphics (Rendering to FBO's that are displayed by ImGUI).
        graphicsHandler->renderGraphics();

        // Assign values to viewport for ImGUI.
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        // Render ImGUI to screen.
        guiHandler.renderGui(io);

        // Swap the OpenGL buffers.
        glfwSwapBuffers(window);
    }

    /*===================================================================================================================================*/

    // Cleanup.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Close application.
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);
    return 0;
}

/*=======================================================================================================================================*/
/* EOF                                                                                                                                   */
/*=======================================================================================================================================*/