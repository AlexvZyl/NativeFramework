#pragma once
#pragma once

/*=======================================================================================================================================*/
/* Includes.                                                                                                                             */
/*=======================================================================================================================================*/

#include <queue>
#include <string>
#include "GUI/GUIState.h"

/*=======================================================================================================================================*/
/* Forward Declerations.                                                                                                                             */
/*=======================================================================================================================================*/

class GUIHandler;
class GraphicsHandler;

/*=======================================================================================================================================*/
/* Data Types.                                                                                                                             */
/*=======================================================================================================================================*/

struct InputQueue
{
    //Constructor
    InputQueue(std::string command, std::string parameters)
        : command(command), parameters(parameters)
    {
    }
    std::string command;
    std::string parameters;
};

struct OutputQueue
{
    //Constructor
    OutputQueue(std::string command, std::string parameters)
        : command(command), parameters(parameters)
    {
    }
    std::string command;
    std::string parameters;
};

/*=======================================================================================================================================*/
/* Class.                                                                                                                                */
/*=======================================================================================================================================*/

class PyInterface
{

public:

    // ----------------------------------------------- //
    //  C O N S T R U C T O R   &   D E S T U C T O R  //
    // ----------------------------------------------- //

    PyInterface(GraphicsHandler* graphicsHandler, GUIHandler* guiHandler, GUIState* guiState);
    ~PyInterface();

    // ------------- //
    //  Q U E U E S  //
    // ------------- //

    std::queue<InputQueue> inputQueueWindow;
    std::queue<InputQueue> inputQueue;
    std::queue<OutputQueue> outputQueue;

    // --------------- //
    //  O B J E C T S  //
    // --------------- //

    GraphicsHandler* m_graphicsHandler;
    GUIHandler* m_guiHandler;
    GUIState* m_guiState;

    // ------------------- //
    //  F U N C T I O N S  //
    // ------------------- //

    void deQueueInput();
    void readingIn();
    void deQueueOutput();
    void readingOut();
    void procesInput(std::string inString);
    void exceptionLog(const std::exception& e);
};

/*=======================================================================================================================================*/
/* EOF.					                                                                                                                 */
/*=======================================================================================================================================*/