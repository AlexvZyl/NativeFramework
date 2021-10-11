/*
API for the graphics of the application.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Class include.
#include "graphicsHandler.h"

// ImGUI (GUI software). 
#include "Implementations/imgui_impl_opengl3.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Add a new window to the app.
void GraphicsHandler::addWindow(std::string windowName, std::string engineType)
{
	// Check if name already used.
	if (isWindowValid(windowName))
	{
		std::cout << "[INTERFACE][ERROR] Name '" << windowName << "' already used.\n\n";
	}
	// Prevent 'NULL' being used as a  name.
	else if (windowName == "NULL")
	{
		std::cout << "[INTERFACE][ERROR] 'NULL' is not a valid name. It is reserved for the inactive state.\n\n";
	}
	else
	{
		// Remove leading white spaces.
		while (engineType[0] == ' ')
		{
			engineType.erase(engineType.begin());
		}
		// Remove white spaces at end.
		while (engineType[engineType.length()-1] == ' ')
		{
			engineType.pop_back();
		}

		// Check engine type.
		if (engineType == "Base" || engineType == "base")
		{
			// Destroy ImGUI to allow drawing in other contexts.
			ImGui_ImplOpenGL3_DestroyDeviceObjects();
			// Set window active.
			m_activeWindow = windowName;
			// Add window to dictionary.
			m_windowsDictionary.insert({ windowName, new RenderWindowGL(m_states, EngineType::BaseEngineGL) });
			m_windowsDictionary[windowName]->windowName = windowName + " [Base]";
		}
		else if (engineType == "Design" || engineType == "design")
		{
			// Destroy ImGUI to allow drawing in other contexts.
			ImGui_ImplOpenGL3_DestroyDeviceObjects();
			// Set window active.
			m_activeWindow = windowName;
			// Add window to dictionary.
			m_windowsDictionary.insert({ windowName, new RenderWindowGL(m_states, EngineType::DesignEngineGL) });
			m_windowsDictionary[windowName]->windowName = windowName + " [Design]";
		}
		// Catch error.
		else 
		{
			std::cout << "[INTERFACE][ERROR] '" << engineType << "' is not a valid engine type.\n\n";
		}
	}
}

// Remove an window from the dict.
void GraphicsHandler::removeWindow(std::string windowName)
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		delete m_windowsDictionary[windowName];
		// Remove by key.
		m_windowsDictionary.erase(windowName);
		// Set window inactive.
		if (m_windowsDictionary.size() != 0)
		{
			m_activeWindow = "NULL";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Draws a line.
void GraphicsHandler::drawLine(std::string windowName, float position1[2], float position2[2], float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			m_windowsDictionary[windowName]->engineGL->drawLine(position1, position2, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Draw clear triangle.
void GraphicsHandler::drawTriangleClear(std::string windowName, float position1[2], float position2[2], float position3[2], float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			m_windowsDictionary[windowName]->engineGL->drawTriangleClear(position1, position2, position3, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Draw filled triangle.
void GraphicsHandler::drawTriangleFilled(std::string windowName, float position1[2], float position2[2], float position3[2], float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			m_windowsDictionary[windowName]->engineGL->drawTriangleFilled(position1, position2, position3, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Draw a clear quad.
void GraphicsHandler::drawQuadClear(std::string windowName, float position[2], float width, float height, float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			m_windowsDictionary[windowName]->engineGL->drawQuadClear(position, width, height, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Draw a filled quad.
void GraphicsHandler::drawQuadFilled(std::string windowName, float position[2], float width, float height, float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			m_windowsDictionary[windowName]->engineGL->drawQuadFilled(position, width, height, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Draws a clear circle.
void GraphicsHandler::drawCircleClear(std::string windowName, float coords[2], float radius, float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			m_windowsDictionary[windowName]->engineGL->drawCircleClear(coords, radius, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Draws a filled circle.
void GraphicsHandler::drawCircleFilled(std::string windowName, float coords[2], float radius, float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			m_windowsDictionary[windowName]->engineGL->drawCircleFilled(coords, radius, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Adds text to the VBO object.
void GraphicsHandler::drawText(std::string windowName, std::string text, float coords[2], float color[4], float scale, std::string align)
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {

			// Remove leading white spaces.
			while (align[0] == ' ')
			{
				align.erase(align.begin());
			}
			// Remove white spaces at end.
			while (align[align.length() - 1] == ' ')
			{
				align.pop_back();
			}

			// Remove leading white spaces.
			while (text[0] == ' ')
			{
				text.erase(text.begin());
			}
			// Remove white spaces at end.
			while (text[text.length() - 1] == ' ')
			{
				text.pop_back();
			}

			// Render the text.
			m_windowsDictionary[windowName]->engineGL->drawText(text, coords, color, scale, align);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Draws the demo to the selected window.
void GraphicsHandler::drawDemo(std::string windowName, unsigned int loopCount)
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Render the demo drawing.
			m_windowsDictionary[windowName]->engineGL->drawDemo(loopCount);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Centers the Drawing around (0,0) and scales it to fit into the window.
void GraphicsHandler::autoCenter(std::string windowName)
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Render the demo drawing.
			m_windowsDictionary[windowName]->engineGL->autoCenter();
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

// Centers the Drawing around (0,0) and scales it to fit into the window.
void GraphicsHandler::updateBuffers(std::string windowName)
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Render the demo drawing.
			m_windowsDictionary[windowName]->engineGL->updateBuffers();
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << windowName << "' is an invalid name.\n\n";
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
