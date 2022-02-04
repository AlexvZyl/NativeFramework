/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include <thread>
#include <chrono>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include "External/Misc/stb_image.h"
#include "External/Misc/ConsoleColor.h"
#include "glad/glad.h"
#include "ImGUI/Core/imgui.h"
#include "ImGUI/Implementations/imgui_impl_opengl3.h"
#include "ImGUI/Implementations/imgui_impl_glfw.h"
#include "GUI/GUIState.h"
#include "GUI/GuiHandler.h"
#include "Graphics/GraphicsHandler.h"
#include "Utilities/PythonInterface/PythonInterface.h"
#include "Application/Application.h"
#include "OpenGL/RendererGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "Resources/ResourceHandler.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"

/*=======================================================================================================================================*/
/* Compiler settings.                                                                                                                    */
/*=======================================================================================================================================*/

// Removes the console in release mode (Windows).
#ifdef _DEBUG
    #pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
#else
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/*=======================================================================================================================================*/
/* Main                                                                                                                                  */
/*=======================================================================================================================================*/

int main(int, char**)
{
    // ----------- //
    //  S E T U P  //
    // ----------- //

    // Init window (GLFW, ImGUI, OpenGL context).
    GLFWwindow* window = Application::glfwInit();
    
    // --------------------------------------------------------------------------------------------------------------//
    // Everything in this section is going to be replaced by the 'Application' class.

    // Create the state machine variables.
    GUIState guiState;

    // Create graphics handler object.
    std::unique_ptr<GraphicsHandler> graphicsHandler = std::make_unique<GraphicsHandler>(&guiState, window);

    // Create a python interfacing object.
    std::unique_ptr<PyInterface> pyInterface;

    // Create GUI handler object.
    std::unique_ptr<GUIHandler> guiHandler = std::make_unique<GUIHandler>(&guiState, graphicsHandler.get(), pyInterface.get());
    pyInterface = std::make_unique<PyInterface>(graphicsHandler.get(), guiHandler.get(), &guiState);
    // --------------------------------------------------------------------------------------------------------------//

    // Create Application instance.
    Application application(window, graphicsHandler.get(), guiHandler.get());

    // Initialise the renderer.
    Renderer::initialise();

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

    // [MAIN LOOP].
    while (!glfwWindowShouldClose(window) && !guiState.globalQuit)
    {
        // Poll commands from python interface.
        pyInterface->deQueueInput();

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

            // Handle the events.
            application.dispatchEvents();

            // Render the frame to the screen.
            application.renderFrame();
            
            // Force push OpenGL commands to the GPU.
            GLCall(glFinish());
            
            // Swap the OpenGL buffers.
            glfwSwapBuffers(window);
        }
    }

    // --------------- //
    //  C L E A N U P  //
    // --------------- //

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
