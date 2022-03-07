//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Application.h"
#include "Utilities/Platform/Preprocessor.h"
#include "Utilities/Interface/ExternalInterface.h"
#include "Utilities/Platform/Thread.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"

//==============================================================================================================================================//
//  Entrypoint.																																	//
//==============================================================================================================================================//

int main(int, char**)
{
    // Initialisation.
    Application application;
    Renderer::initialise();
    std::thread externalInput(ExternalInterface::inputThread);

    // Main loop.
    application.run();

    // Cleanup.
    lumenTerminateThread(externalInput);
    Renderer::shutdown();
    application.shutdown();

    return 0;
}

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//
