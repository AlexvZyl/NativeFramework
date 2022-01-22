/*
API for the graphics of the application.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Class include.
#include "GraphicsHandler.h"
// ImGUI. 
#include "Implementations/imgui_impl_opengl3.h"
// General
#include <stdio.h>
#include <External/Misc/ConsoleColor.h>
#include "OpenGL/RendererGL.h"

//=============================================================================================================================================//
//  Window Handling.																														   //
//=============================================================================================================================================//

// Add a new window to the app.
void GraphicsHandler::addWindow(std::string windowName, std::string engineType)
{
	// Check if name already used.
	if (isWindowValid(windowName))
	{
		std::cout << red << "\n[INTERFACE] [ERROR] : " << white << "Name '" << windowName << "' already used.\n";
	}
	else
	{
		// Remove leading white spaces.
		while (engineType[0] == ' ')					   { engineType.erase(engineType.begin());	}
		// Remove white spaces at end.
		while (engineType[engineType.length() - 1] == ' ') { engineType.pop_back();					}

		// Check engine type.
		if (engineType == "Base2D" || engineType == "base2D")
		{
			// Destroy ImGUI to allow drawing in other contexts.
			ImGui_ImplOpenGL3_DestroyDeviceObjects();
			// Add window to dictionary.
			m_windowsDictionary.insert({ windowName, std::make_shared<RenderWindowGL>(m_guiState, EngineType::Base2DEngine, windowName) });
			// Set active window.
			m_activeWindow = m_windowsDictionary[windowName];
		}
		else if (engineType == "Design2D" || engineType == "design2D")
		{
			// Destroy ImGUI to allow drawing in other contexts.
			ImGui_ImplOpenGL3_DestroyDeviceObjects();
			// Add window to dictionary.
			m_windowsDictionary.insert({ windowName, std::make_shared<RenderWindowGL>(m_guiState, EngineType::Design2DEngine, windowName) });
			// Set active window.
			m_activeWindow = m_windowsDictionary[windowName];
			m_guiState->design_engine = reinterpret_cast<Design2DEngine*>(m_activeWindow->engineGL.get());
		}
		else if (engineType == "Base3D" || engineType == "base3D")
		{
			// Destroy ImGUI to allow drawing in other contexts.
			ImGui_ImplOpenGL3_DestroyDeviceObjects();
			// Add window to dictionary.
			m_windowsDictionary.insert({ windowName, std::make_shared<RenderWindowGL>(m_guiState, EngineType::Base3DEngine, windowName) });
			// Set active window.
			m_activeWindow = m_windowsDictionary[windowName];
		}
		// Catch error.
		else
		{
			std::cout << red << "\n[INTERFACE] [ERROR] : " << white << " '" << engineType << "' is not a valid engine type.\n";
		}
	}
	m_activeWindow->resizeEvent = true;
	m_guiState->renderResizeEvent = true;
}

// Remove an window from the dict.
void GraphicsHandler::removeWindow(std::string windowName)
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		// Remove by key.
		m_windowsDictionary.erase(windowName);
		// If the dict is empty make the window incavtive.
		if (m_windowsDictionary.size() != 0) { m_activeWindow = NULL; }
	}
	else { windowError(windowName); }
}

//=============================================================================================================================================//
//  General rendering functions.																											   //
//=============================================================================================================================================//

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
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
}

//=============================================================================================================================================//
// 2D Rendering.																															   //
//=============================================================================================================================================//


// Draws a line.
void GraphicsHandler::drawLine(std::string windowName, float position1[2], float position2[2], float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			Renderer::bindScene(m_windowsDictionary[windowName]->engineGL->m_scene.get());
			Renderer::addLineSegment2D(glm::vec2(position1[0], position1[1]), glm::vec2(position2[0], position2[1]), NULL);
		}
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
}

// Draw clear triangle.
void GraphicsHandler::drawTriangleClear(std::string windowName, float position1[2], float position2[2], float position3[2], float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			//m_windowsDictionary[windowName]->engineGL->drawTriangleClear(position1, position2, position3, color);
		}
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
}

// Draw filled triangle.
void GraphicsHandler::drawTriangleFilled(std::string windowName, float position1[2], float position2[2], float position3[2], float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			//m_windowsDictionary[windowName]->engineGL->drawTriangleFilled(position1, position2, position3, color);
		}
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
}

// Draw a clear quad.
void GraphicsHandler::drawQuadClear(std::string windowName, float position[2], float width, float height, float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			//m_windowsDictionary[windowName]->engineGL->drawQuadClear(position, width, height, color);
		}
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
}

// Draw a filled quad.
void GraphicsHandler::drawQuadFilled(std::string windowName, float position[2], float width, float height, float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			//m_windowsDictionary[windowName]->engineGL->drawQuadFilled(position, width, height, color);
		}
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
}

// Draws a clear circle.
void GraphicsHandler::drawCircleClear(std::string windowName, float coords[2], float radius, float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			//m_windowsDictionary[windowName]->engineGL->drawCircleClear(coords, radius, color);
		}
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
}

// Draws a filled circle.
void GraphicsHandler::drawCircleFilled(std::string windowName, float coords[2], float radius, float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			//m_windowsDictionary[windowName]->engineGL->drawCircleFilled(coords, radius, color);
		}
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
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
			//m_windowsDictionary[windowName]->engineGL->drawText(text, coords, color, scale, align);
		}
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
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
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
}

// ===================================================================================================================== //
// 3D Rendering.																										 //
// ===================================================================================================================== //

void GraphicsHandler::drawQuadFilled3D(std::string windowName, float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			//m_windowsDictionary[windowName]->engineGL->drawQuadFilled3D(vertex1, vertex2, vertex3, vertex4, color);
		}
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
}

void GraphicsHandler::drawCuboidFilled(std::string windowName, float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float depth, float color[4])
{
	// Check if window exists.
	if (isWindowValid(windowName))
	{
		try {
			// Call BaseEngine function.
			//m_windowsDictionary[windowName]->engineGL->drawCuboidFilled(vertex1, vertex2, vertex3, vertex4, depth, color);
		}
		catch (const std::exception& e) { parametersError(e); }
	}
	else { windowError(windowName); }
}

// ===================================================================================================================== //
// Error handling.																										 //
// ===================================================================================================================== //

// Print error messages.
void GraphicsHandler::windowError(std::string windowName)
{
	std::cout << red << "\n[INTERFACE] [ERROR] : " << white << "'" << windowName << "' is an invalid name.\n";
}
void GraphicsHandler::parametersError(const std::exception& e)
{
	std::cout << red << "\n[INTERFACE] [ERROR] : " << white << "Invalid parameters caused exception : '" << e.what() << "'.\n";
}

// ===================================================================================================================== //
// EOF.																													 //
// ===================================================================================================================== //