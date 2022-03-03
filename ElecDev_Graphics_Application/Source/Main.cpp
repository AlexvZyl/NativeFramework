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

    // FPS cap settings.
    double fps = 60;
    double targetFrameTime = 1 / fps;
    double totalFrameTime = 0;
    double currTime = 0;
    double prevTime = 0;

    // Waiting for events.
    bool wait = false;

    // [MAIN LOOP].
    while (!application.shouldWindowClose())
    {
        // Event checking.
        if (wait) glfwWaitEventsTimeout(targetFrameTime * 4);
        else      glfwPollEvents();
     
        // Frametime calculations.
        currTime = glfwGetTime();
        totalFrameTime += currTime - prevTime;
        prevTime = currTime;

        // Render screen with fps cap.
        if (totalFrameTime > targetFrameTime) 
        {
            // Reset frametime.
            totalFrameTime = 0;

            // Render the frame to the screen,
            // as well as handle events.
            application.onRender();
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
