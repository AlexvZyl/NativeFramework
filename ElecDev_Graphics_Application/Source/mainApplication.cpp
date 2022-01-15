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
#include "ImGUI/Implementations/imgui_impl_glfw.h"
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
    // ----------- //
    //  S E T U P  //
    // ----------- //

    // Init window (GLFW, ImGUI, OpenGL context).
    GLFWwindow* window = initWindow();

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

    // ------------------------------- //
    //  R E N D E R   P I P E L I N E  //
    // ------------------------------- //

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

    ImGuiIO& io = ImGui::GetIO(); (void)io;

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