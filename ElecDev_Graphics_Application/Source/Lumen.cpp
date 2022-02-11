/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include <chrono>
#include "OpenGL/RendererGL.h"
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

    // Init the window.
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

    // [MAIN LOOP].
    while (!application.shouldWindowClose())
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

            // Clear buffers.
            Renderer::clear();

            // Render the frame to the screen,
            // as well as handle events.
            application.onRender();
            
            // Force push commands to the GPU.
            Renderer::finish();
            
            // Swap the window buffers.
            application.swapBuffers();
        }
    }

    // --------------- //
    //  C L E A N U P  //
    // --------------- //

    application.shutdown();
    return 0;
}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/
