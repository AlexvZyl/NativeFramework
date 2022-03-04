//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/RendererGL.h"
#include "Preprocessor.h"
#include "Application/Application.h"
#include "Utilities/Interface/ExternalInterface.h"
#include <thread>
#include "External/GLFW/Includes/GLFW/glfw3.h"

//==============================================================================================================================================//
//  Entrypoint.																																	//
//==============================================================================================================================================//

int main(int, char**)
{
    // Window, OpenGL context, GLAD.
    GLFWwindow* window = Application::glfwInitWindow();

    // Initialise the renderer.
    Renderer::initialise();

    // Create an application instance inside GLFW window.
    Application application(window);

    // Start interface thread.
    std::thread externalInput(ExternalInterface::inputThread);

    // Run the application.
    application.run();

    // Cleanup.
    externalInput.join();
    return 0;
}

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//
