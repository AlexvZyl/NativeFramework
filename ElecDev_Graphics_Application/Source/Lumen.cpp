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
#include "OpenGL/RendererGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "Resources/ResourceHandler.h"
#include "Application/Events/EventLog.h"
#include "Application/Layers/LayerStack.h"
#include "Application/Application.h"
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
    GLFWwindow* window = Application::glfwInitWindow();

    // Initialise the renderer.
    Renderer::initialise();

    // Create Application instance.
    Application application(window);

    // ------------------------------------- //
    //  R E N D E R I N G   P I P E L I N E  //
    // ------------------------------------- //

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
    while (!glfwWindowShouldClose(window))
    {
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
            Renderer::clear();

            // Handle the events.
            application.dispatchEvents();

            // Render the frame to the screen.
            application.onRender();
            
            // Force push commands to the GPU.
            Renderer::finish();
            
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
