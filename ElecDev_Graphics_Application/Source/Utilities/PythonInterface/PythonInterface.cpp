/*=======================================================================================================================================*/
/* Includes.					                                                                                                         */
/*=======================================================================================================================================*/

#include "Utilities/PythonInterface/PythonInterface.h" 
#include <stdio.h>
#include <External/Misc/ConsoleColor.h>
#include "GUI/GUIState.h"
#include "Graphics/GraphicsHandler.h"
#include "GUI/GuiHandler.h"
#include "GUI/UserGUI/userGUI.h"

/*=======================================================================================================================================*/
/* Constructor & Destructor.                                                                                                             */
/*=======================================================================================================================================*/

PyInterface::PyInterface(GraphicsHandler* graphicsHandler, GUIHandler* guiHandler, GUIState* guiState)
    : m_graphicsHandler(graphicsHandler), m_guiHandler(guiHandler), m_guiState(guiState)
{
}

PyInterface::~PyInterface()
{
    delete[] &inputQueueWindow;  // Is this correct?...
}

/*=======================================================================================================================================*/
/* Processing Input.                                                                                                                     */
/*=======================================================================================================================================*/

// Hash the input text to be checked against list of commands.
constexpr size_t hash(const char* str)
{
    const long long p = 131;
    const long long m = 4294967291; // 2^32 - 5, largest 32 bit prime.
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
void PyInterface::procesInput(std::string inString)
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
        m_guiState->globalQuit = true;
        break;
    }

    // Command to be processed by the app.
    case hash("Command"):
    {
        std::string function = inString.substr(1, inString.find("]") - 1);
        inString = inString.substr(inString.find("]") + 1);
        std::string params = inString.substr(1, inString.find("]") - 1);
        inputQueueWindow.push(*(new InputQueue(function, params)));
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

/*=======================================================================================================================================*/
/* Threading .                                                                                                                           */
/*=======================================================================================================================================*/

// Threaded function that reads the messages from python.
void PyInterface::readingIn()
{
    std::string temp;
    while (true)
    {
        std::getline(std::cin, temp);
        if (temp != "")
        {
            std::cout << blue << "[INTERFACE] [INFO] : " << white << "Command received." << std::endl;
            std::string subCommand = "";
            while (true)
            {
                int nextStart = temp.find("::");
                if (nextStart == -1)
                {
                    break;
                }
                subCommand = temp.substr(0, nextStart);
                procesInput(subCommand);
                temp = temp.substr(nextStart + 2, temp.size());
            }
        }
        temp = "";
    }
}

void PyInterface::readingOut()
{
    while (true) { deQueueOutput(); }
}

/*=======================================================================================================================================*/
/* Queue handling.                                                                                                                       */
/*=======================================================================================================================================*/

void PyInterface::deQueueOutput()
{
    while (outputQueue.size() > 0)
    {
        OutputQueue temp = outputQueue.front();
        std::cout << temp.command << temp.parameters << std::endl;
        outputQueue.pop();
    }
}

// Process the funcions laoded by the python thread.
void PyInterface::deQueueInput() 
{

    // Run while there are commands left to process.
    while (inputQueueWindow.size() > 0) 
    {
        // Processing variables.
        InputQueue temp = inputQueueWindow.front();
        std::string mccName;
        std::string text;
        std::string align;
        std::vector<float> params;
        std::string guiName;
        std::string guiPos;
        std::string docking;
        std::string parameters;

        // Try the command and catch exceptions.
        try {

            // Switch between different commands.
            switch (hash(temp.command.c_str()))
            {
                // Draw line.
            case hash("DrawLine"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 8; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                m_graphicsHandler->drawLine(mccName, new float[2]{ params[0],params[1] }, new float[2]{ params[2],params[3] }, new float[4]{ params[4],params[5],params[6],params[7] });
                break;

                // Draw clear triangle.
            case hash("DrawTriangleClear"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 10; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                m_graphicsHandler->drawTriangleClear(mccName, new float[2]{ params[0],params[1] }, new float[2]{ params[2],params[3] }, new float[2]{ params[4], params[5] }, new float[4]{ params[6],params[7],params[8],params[9] });
                break;

                // Draw filled triangle. 
            case hash("DrawTriangleFilled"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 10; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                m_graphicsHandler->drawTriangleFilled(mccName, new float[2]{ params[0],params[1] }, new float[2]{ params[2],params[3] }, new float[2]{ params[4], params[5] }, new float[4]{ params[6],params[7],params[8],params[9] });
                break;

                // Draw clear quad.
            case hash("DrawQuadClear"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 8; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                m_graphicsHandler->drawQuadClear(mccName, new float[2]{ params[0],params[1] }, params[2], params[3], new float[4]{ params[4],params[5],params[6],params[7] });
                break;

                // Draw filled quad.
            case hash("DrawQuadFilled"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 8; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                m_graphicsHandler->drawQuadFilled(mccName, new float[2]{ params[0],params[1] }, params[2], params[3], new float[4]{ params[4],params[5],params[6],params[7] });
                break;

                // Draw clear circle.
            case hash("DrawCircleClear"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 7; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                m_graphicsHandler->drawCircleClear(mccName, new float[2]{ params[0],params[1] }, params[2], new float[4]{ params[3],params[4],params[5],params[6] });
                break;

                // Draw a filled circle.
            case hash("DrawCircleFilled"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 7; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                m_graphicsHandler->drawCircleFilled(mccName, new float[2]{ params[0],params[1] }, params[2], new float[4]{ params[3],params[4],params[5],params[6] });
                break;

                // Draw text.
            case hash("DrawText"):
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
                m_graphicsHandler->drawText(mccName, text, new float[2]{ params[0],params[1] }, new float[4]{ params[2],params[3],params[4], params[5] }, params[6], align);
                break;

                // Add MCC window to draw.
            case hash("AddWindow"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                text = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                m_graphicsHandler->addWindow(mccName, text);
                break;

                // Remove MCC window.
            case hash("RemoveWindow"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                m_graphicsHandler->removeWindow(mccName);
                break;

                // Draw the demo.
            case hash("DrawDemo"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                m_graphicsHandler->drawDemo(mccName, (unsigned int)params[0]);
                break;

                // Center the drawing around (0,0).
            case hash("AutoCenter"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                m_graphicsHandler->autoCenter(mccName);
                break;

                // Load CPU buffers to GPU.
            case hash("UpdateDrawing"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                m_graphicsHandler->updateBuffers(mccName);
                break;

            case hash("addGUI"):
                guiName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                guiPos = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                docking = temp.parameters.substr(0, temp.parameters.find(";"));
                parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                m_guiHandler->m_userGUIP->createGUI(guiName, guiPos, docking, parameters);
                break;

            /*===================================================================================================================================*/
            /* 3D API.                                                                                                                           */
            /*===================================================================================================================================*/

                // Draw filled quad.
            case hash("DrawQuadFilled3D"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 16; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                m_graphicsHandler->drawQuadFilled3D(mccName, new float[3]{ params[0], params[1], params[2] },
                    new float[3]{ params[3], params[4], params[5] },
                    new float[3]{ params[6], params[7], params[8] },
                    new float[3]{ params[9], params[10], params[11] },
                    new float[4]{ params[12],params[13],params[14],params[15] });
                break;

                // Draw filled quad.
            case hash("DrawCuboidFilled"):
                mccName = temp.parameters.substr(0, temp.parameters.find(";"));
                temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                for (size_t i = 0; i < 17; i++)
                {
                    params.push_back(std::stof(temp.parameters.substr(0, temp.parameters.find(";"))));
                    temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
                }
                m_graphicsHandler->drawCuboidFilled(mccName, new float[3]{ params[0], params[1], params[2] },
                    new float[3]{ params[3], params[4], params[5] },
                    new float[3]{ params[6], params[7], params[8] },
                    new float[3]{ params[9], params[10], params[11] },
                    params[12],
                    new float[4]{ params[13],params[14],params[15],params[16] });
                break;

            default:
                std::cout << red << "\n[INTERFACE] [ERROR] : " << white << "'" << temp.command.c_str() << "' function invalid. \n";
                break;
            }

            std::cout << green << "\n[ELECDEV] [INPUT] : " << white;

            // Remove command from queue.
            inputQueueWindow.pop();
        }
        // Catch error and print log.              
        catch (const std::exception& e)
        {
            exceptionLog(e);
        }
    }
}

// Print the exception message to the terminal.
void PyInterface::exceptionLog(const std::exception& e)
{
    std::cout << red << "\n[INTERFACE] [ERROR] : " << white << "Invalid parameters caused exception : '" << e.what() << "'.\n";
    std::cout << green << "\n[ELECDEV] [INPUT] : " << white;
}

/*=======================================================================================================================================*/
/* EOF.					                                                                                                                 */
/*=======================================================================================================================================*/