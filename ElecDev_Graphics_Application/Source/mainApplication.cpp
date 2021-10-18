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

// Resources.
#include <Misc/stb_image.h>
#include "Resources/ResourceHandler.h"

// Console coloring.
#include "External/Misc/ConsoleColor.h"

// GUI includes.
#include "GUI/guiHandler.h"

// Graphics handler include.
#include <../Graphics/GraphicsHandler.h>

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
/* Python interface declarations.                                                                                                        */
/*=======================================================================================================================================*/

void deQueueInput(stateMachine* states);
void readingIn(stateMachine* states);
void deQueueOutput(stateMachine* states);
void readingOut(stateMachine* states);
constexpr size_t hash(const char* str);
void procesInput(std::string inString, stateMachine* states);
void exceptionLog(const std::exception& e);

/*=======================================================================================================================================*/
/* Variables/Globals/Defines.                                                                                                            */
/*=======================================================================================================================================*/

// Defined here, assigned in the main function where it has access to the window.
// Used as a global variable so that the mouse event callbacks from GLFW can have
// access to it.
GraphicsHandler* graphicsHandler;
GUIHandler* guiHandler;

/*=======================================================================================================================================*/
/* GLFW callbacks.                                                                                                                       */
/*=======================================================================================================================================*/

// GLFW error handler.
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, (const char*)red, "\n\n[GLFW] [ERROR] : ", (const char*)white,  "%d: %s\n", error, description);
}

/*=======================================================================================================================================*/
/* Mouse events callbacks.                                                                                                               */
/*=======================================================================================================================================*/

// Handle mouse press events from GLFW.
void mousePressEvent(GLFWwindow* window, int button, int action, int mods)
{
    graphicsHandler->mousePressEvent(button, action);
}

// Handle mouse press events from GLFW.
void mouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{   
    // Get button state.
    int buttonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    graphicsHandler->mouseMoveEvent(buttonState);
}

// Handle mouse press scroll. from GLFW.
void mouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
    graphicsHandler->mouseScrollEvent((float)yoffset);
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
        const char* glsl_version = "#version 460";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    glfwSwapInterval(1); // VSync buffer size.

    // Load GLFW icon.
    BITMAP bitmap = loadImageFromResource(ICON_PNG);
    GLFWimage icon;
    // Load bitmap data to GLFW icon.
    icon.pixels = (unsigned char*) bitmap.bmBits;
    icon.height = bitmap.bmHeight;
    icon.width = bitmap.bmWidth;
    // Set icon.
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
        fprintf(stderr, (const char*)red, "\n\n[OPENGL] [ERROR] : ", (const char*)white,   " Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Print OpenGL version.
    std::cout << blue << "\n[OPENGL] [INFO] : " << white << " Loaded OpenGL version : " << glGetString(GL_VERSION) << ".";

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
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
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
    GLCall(glViewport(0, 0, screen_width, screen_height));

    // Setup mouse callbacks.
    glfwSetMouseButtonCallback(window, mousePressEvent);    // Mouse press event.
    glfwSetCursorPosCallback(window, mouseMoveEvent);       // Mouse move event.
    glfwSetScrollCallback(window, mouseScrollEvent);        // Mouse scroll event.

    // OpenGL inits.
    GLCall(glEnable(GL_MULTISAMPLE));       // MSAA.
    GLCall(glEnable(GL_DEPTH_TEST));        // Depth testing (the z buffer).

    // Create the state machine variables.
    stateMachine* states = new stateMachine();
    states->toolsExpanded = false;
    states->toolsMode = 0;

    // Create graphics handler object.
    // For now a global variable is used to be able to have mouse callbacks with a method.
    // The callbacks cannot be used with a method, so it has to call a normal function.
    graphicsHandler = new GraphicsHandler(states);

    // Create GUI handler object.
    guiHandler = new GUIHandler(states, graphicsHandler);

    /*===================================================================================================================================*/
    /* Loop                                                                                                                              */
    /*===================================================================================================================================*/

    // Loop variables.
    bool wait = false;          // Wait for events.
    int display_w, display_h;   // Viewport for ImGUI.

    // Thread reading inputs from pipeline.
    std::thread t1(readingIn, states);

    //Thread writing to the pipeline
    std::thread t2(readingOut, states);

    // [MAIN LOOP] Graphics Pipeline
    while (!glfwWindowShouldClose(window) && !states->globalQuit)
    {
        // Poll commands from python interface.
        deQueueInput(states);

        // Event checking.
        if (wait) { glfwWaitEvents(); }   // App only runs when events occur.
        else { glfwPollEvents(); }        // App runs continuously.
    
        // Clear colors for OpenGL.
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
     
        // Handle graphics (Rendering to FBO's that are displayed by ImGUI).
        graphicsHandler->renderGraphics();

        // Assign values to viewport for ImGUI.
        glfwGetFramebufferSize(window, &display_w, &display_h);
        GLCall(glViewport(0, 0, display_w, display_h));

        // Render ImGUI to screen.
        guiHandler->renderGui(io);

        // Swap the OpenGL buffers.
        glfwSwapBuffers(window);

    }

    /*===================================================================================================================================*/

    // Cleanup.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Log exiting.
    std::cout << blue << "\n\n[ELECDEV] [INFO] : " << white << "Program terminated." << std::endl;

    // Close application.
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);
    return 0;
}

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
void procesInput(std::string inString, stateMachine* states) 
{
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

        // Error output of the command is invalid.
        default:
        {
            std::cout << red << "\n[INTERFACE] [ERROR] : " << white << " '" << command << "' type invalid.\n";
            std::cout << green << "\n[ELECDEV] [INPUT] : " << white;
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
            std::cout << "Received" << std::endl;
            std::string subCommand = "";
            while (true) {
                int nextStart = temp.find("::");
                if (nextStart == -1) {
                    break;
                }
                subCommand = temp.substr(0, nextStart);
                procesInput(subCommand, states);
                temp = temp.substr(nextStart + 2, temp.size());
                
            }
            
        }
        temp = "";
    }
}

void readingOut(stateMachine* states) {

    while (true) {
        deQueueOutput(states);
    }
}

void deQueueOutput(stateMachine* states) {

    while (states->outputQ.size() > 0) {
        outputQueue temp = states->outputQ.front();

        std::cout << temp.command << temp.parameters << std::endl;

        states->outputQ.pop();

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
        std::string align;
        std::vector<float> params;

        std::string guiName;
        std::string guiPos;
        std::string parameters;


        // Try the command and catch exceptions.
        try {

            // Switch between different commands.
            switch (hash(temp.command.c_str()))
            {
                // Draw line.
            case hash("drawLine"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 8; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                graphicsHandler->drawLine(mccName, new float[2]{ params[0],params[1] }, new float[2]{ params[2],params[3] }, new float[4]{ params[4],params[5],params[6],params[7] });
                break;

                // Draw clear triangle.
            case hash("drawTriangleClear"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 10; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                graphicsHandler->drawTriangleClear(mccName, new float[2]{ params[0],params[1] }, new float[2]{ params[2],params[3] }, new float[2]{ params[4], params[5] }, new float[4]{ params[6],params[7],params[8],params[9] });
                break;

                // Draw filled triangle. 
            case hash("drawTriangleFilled"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 10; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                graphicsHandler->drawTriangleFilled(mccName, new float[2]{ params[0],params[1] }, new float[2]{ params[2],params[3] }, new float[2]{ params[4], params[5] }, new float[4]{ params[6],params[7],params[8],params[9] });
                break;

                // Draw clear quad.
            case hash("drawQuadClear"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 8; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                graphicsHandler->drawQuadClear(mccName, new float[2]{ params[0],params[1] }, params[2], params[3], new float[4]{ params[4],params[5],params[6],params[7] });
                break;

                // Draw filled quad.
            case hash("drawQuadFilled"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 8; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                graphicsHandler->drawQuadFilled(mccName, new float[2]{ params[0],params[1] }, params[2], params[3], new float[4]{ params[4],params[5],params[6],params[7] });
                break;

                // Draw clear circle.
            case hash("drawCircleClear"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 7; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                graphicsHandler->drawCircleClear(mccName, new float[2]{ params[0],params[1] }, params[2], new float[4]{ params[3],params[4],params[5],params[6] });
                break;

                // Draw a filled circle.
            case hash("drawCircleFilled"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 7; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                graphicsHandler->drawCircleFilled(mccName, new float[2]{ params[0],params[1] }, params[2], new float[4]{ params[3],params[4],params[5],params[6] });
                break;

                // Draw text.
            case hash("drawText"):
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
                    // Read tex alignment.
                    if (i == 7)
                    {
                        align = temp.parameters.substr(0, temp.parameters.find(";"));
                        temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    }
                    // Read parameters.
                    else
                    {
                        params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                        temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                    }
                }
                graphicsHandler->drawText(mccName, text, new float[2]{ params[0],params[1] }, new float[4]{ params[2],params[3],params[4], params[5] }, params[6], align);
                break;

                // Add MCC window to draw.
            case hash("addWindow"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                text = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                graphicsHandler->addWindow(mccName, text);
                break;

                // Remove MCC window.
            case hash("removeWindow"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                graphicsHandler->removeWindow(mccName);
                break;

                // Draw the demo.
            case hash("drawDemo"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                graphicsHandler->drawDemo(mccName, (unsigned int)params[0]);
                break;

                // Center the drawing around (0,0).
            case hash("autoCenter"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                graphicsHandler->autoCenter(mccName);
                break;

                // Load CPU buffers to GPU.
            case hash("updateBuffers"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                graphicsHandler->updateBuffers(mccName);
                break;

            case hash("addGUI"):
                std::cout << "GUI" << std::endl;
                guiName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                guiPos = temp.parameters.substr(0, temp.parameters.find(";"));
                parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                guiHandler->createGUI(guiName, guiPos, parameters);
                break;

            default:
                std::cout << red << "\n[INTERFACE] [ERROR] : " << white << "'" << temp.command.c_str() << "' function invalid. \n";
                break;
            }

            std::cout << green << "\n[ELECDEV] [INPUT] : " << white;

            // Remove command from queue.
            states->inputQueueMCC.pop();
        }
        // Catch error and print log.              
        catch (const std::exception& e)
        {
            exceptionLog(e);
        }
    }
}


// Print the exception message to the terminal.
void exceptionLog(const std::exception& e) 
{
    std::cout << red << "\n[INTERFACE] [ERROR] : " << white << "Invalid parameters caused exception : '" << e.what() << "'.\n";
    std::cout << green << "\n[ELECDEV] [INPUT] : " << white;
}

/*=======================================================================================================================================*/
/* EOF                                                                                                                                   */
/*=======================================================================================================================================*/