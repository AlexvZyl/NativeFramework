/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

// General.
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
// ImGUI (GUI software). 
#include "Core/imgui.h"
#include "Implementations/imgui_impl_opengl3.h"
// GLAD (OpenGL loader).
#include <glad/glad.h>
// Resources.
#include <Misc/stb_image.h>
#include "Resources/ResourceHandler.h"
// Console coloring.
#include "External/Misc/ConsoleColor.h"
// Gui states.
#include "GUI/GUIState.h"
// General.
#include <thread>
// Handlers.
#include "Graphics/GraphicsHandler.h"
#include "GUI/GuiHandler.h"
#include "Utilities/PythonInterface/PythonInterface.h"
// Include GLFW (window) after OpenGL definition.
#include "Application/Events/EventsImplGLFW.h"
// Measure time.
#include <chrono>
// Include the renderer for initialisartion.
#include "CoreGL/RendererGL.h"

/*=======================================================================================================================================*/
/* Compiler settings.                                                                                                                    */
/*=======================================================================================================================================*/

// Removes the console in release mode.
#ifdef	_DEBUG
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
#else
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/*=======================================================================================================================================*/
/* Handler declerations.					                                                                                              /
/*=======================================================================================================================================*/

// Defined here, assigned in the main function where it has access to the window.
std::unique_ptr<GraphicsHandler> graphicsHandler;
std::unique_ptr<GUIHandler> guiHandler;


/*=======================================================================================================================================*/
/* Main                                                                                                                                  */
/*=======================================================================================================================================*/

int main(int, char**)
{
    // ------------- //
    //  W I N D O W  //
    // ------------- //

    // Init GLFW.
    GLFWwindow* window = initGLFW();

    // --------------------------- //
    //  O P E N G L   L O A D E R  //
    // --------------------------- //

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
    std::cout << blue << "\n[OPENGL] [INFO] : " << white << " Loaded OpenGL version " << glGetString(GL_VERSION) << ".";

    // ----------------------------------------- //
    //  I M G U I   &   O P E N G L   S E T U P  // 
    // ----------------------------------------- //

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;                                    // Disable imgui ini file.
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable keyboard controls.
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // When viewports are enabled we tweak WindowRounding/WindowBg 
    // so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // OpenGL settings.
    GLCall(glEnable(GL_MULTISAMPLE));                           // Enables MSAA.
    GLCall(glEnable(GL_DEPTH_TEST));                            // Enables depth testing for the OpenGL scenes.
    GLCall(glDepthFunc(GL_LESS));                               // Set the function used with depth testing.
    GLCall(glEnable(GL_BLEND));                                 // Enable blending for alpha channels.
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  // Set blend function.  This is the standard setting.
    GLCall(glClearColor(0.08f, 0.08f, 0.10f, 1.00f));           // Set the color to which OpenGL clears.
    
    // Initialise the renderer.
    Renderer::initialise();

    // Create the state machine variables.
    GUIState guiState;

    // Create graphics handler object.
    // For now a global variable is used to be able to have mouse callbacks with a method.
    // The callbacks cannot be used with a method, so it has to call a normal function.
    graphicsHandler = std::make_unique<GraphicsHandler>(&guiState, window);

    // Create a python interfacing object.
    PyInterface pyInterface(graphicsHandler.get(), guiHandler.get(), &guiState);

    // Create GUI handler object.
    guiHandler = std::make_unique<GUIHandler>(&guiState, graphicsHandler.get(), &pyInterface);

    /*===================================================================================================================================*/
    /* Render pipeline.                                                                                                                  */
    /*===================================================================================================================================*/


    // Thread reading inputs from pipeline.
    //std::thread t1(&PyInterface::readingIn, &pyInterface);
    // Thread writing to the pipeline.
    //std::thread t2(&PyInterface::readingOut, &pyInterface);

    // Set waiting for events.
    bool wait = false;

    // FPS cap settings.
    double fps = 60;
    double targetFrameTime = 1 / fps;
    double totalFrameTime = 0;
    double currTime = 0;
    double prevTime = 0;

    // Input message.
    std::cout << green << "\n[LUMEN] [INPUT] : " << white;

    // Reset glfw time.
    glfwSetTime(0);

    // [MAIN LOOP].
    while (!glfwWindowShouldClose(window) && !guiState.globalQuit)
    {
        // Poll commands from python interface.
        pyInterface.deQueueInput();

        // Event checking.
        if (wait) { glfwWaitEvents(); }   // App only runs when events occur.
        else      { glfwPollEvents(); }   // App runs continuously.
     
        // Frametime calculations.
        currTime = glfwGetTime();
        totalFrameTime += currTime - prevTime;
        prevTime = currTime;

        // Render screen with fps cap.
        if (totalFrameTime > targetFrameTime) 
        {
            // Reset frametime.
            totalFrameTime = 0;

            // Clear buffers for OpenGL.
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // Handle graphics (Rendering to FBO's that are displayed by ImGUI).
            graphicsHandler->renderLoop();

            // Render ImGUI to screen.
            guiHandler->renderGui(io, window);
            
            // Swap the OpenGL buffers.
            glfwSwapBuffers(window);

            // Force push OpenGL commands to the GPU.
            GLCall(glFinish());
        }
    }

    /*===================================================================================================================================*/

    // ImGUI cleanup.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext();

    // Close application.
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);

    // Log termination.
    std::cout << blue << "\n\n[LUMEN] [INFO] : " << white << "Program terminated." << std::endl;
    return 0;
}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/