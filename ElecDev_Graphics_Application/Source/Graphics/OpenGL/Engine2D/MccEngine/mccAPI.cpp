/*
Engine API.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "mccEngineCore.h"

// ImGUI (GUI software). 
#include "Implementations/imgui_impl_opengl3.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Add MCC to the dictS.
void MccEngineGL::addMCC(std::string mccName)
{
	// Check if name already used.
	if (isNameInDictionary(mccName)) 
	{
		std::cout << "[INTERFACE][ERROR] Name '" << mccName <<"' already used.\n\n";
	}
	// Prevent 'NONE' being used as a  name.
	else if (mccName == "NULL") 
	{
		std::cout << "[INTERFACE][ERROR] 'NULL' is not a valid name. It is reserved for the inactive state.\n\n";
	}
	else 
	{
		// Destroy ImGUI to allow drawing in other contexts.
		ImGui_ImplOpenGL3_DestroyDeviceObjects();
		// Add to dictionary.
		m_mccDictionary.insert({ mccName, new MccStruct(m_states) });
		// Set MCC name.
		m_activeMCC = mccName;
	}
}

// Remove an MCC from the dict.
void MccEngineGL::removeMCC(std::string mccName)
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName)) 
	{
		delete m_mccDictionary[mccName];
		// Remove by key.
		m_mccDictionary.erase(mccName);
		// Set new active MCC.
		if (m_mccDictionary.size() != 0)
		{
			m_activeMCC = "NULL";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Draws a line.
void MccEngineGL::drawLine(std::string mccName, float position1[2], float position2[2], float color[4])
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Call BaseEngine function.
			m_mccDictionary[mccName]->engine->drawLine(position1, position2, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" <<  e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Draw clear triangle.
void MccEngineGL::drawTriangleClear(std::string mccName, float position1[2], float position2[2], float position3[2], float color[4])
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Call BaseEngine function.
			m_mccDictionary[mccName]->engine->drawTriangleClear(position1, position2, position3, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Draw filled triangle.
void MccEngineGL::drawTriangleFilled(std::string mccName, float position1[2], float position2[2], float position3[2], float color[4])
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Call BaseEngine function.
			m_mccDictionary[mccName]->engine->drawTriangleFilled(position1, position2, position3, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Draw a clear quad.
void MccEngineGL::drawQuadClear(std::string mccName, float position[2], float width, float height, float color[4])
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Call BaseEngine function.
			m_mccDictionary[mccName]->engine->drawQuadClear(position, width, height, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Draw a filled quad.
void MccEngineGL::drawQuadFilled(std::string mccName, float position[2], float width, float height, float color[4])
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Call BaseEngine function.
			m_mccDictionary[mccName]->engine->drawQuadFilled(position, width, height, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Draws a clear circle.
void MccEngineGL::drawCircleClear(std::string mccName, float coords[2], float radius, float color[4])
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Call BaseEngine function.
			m_mccDictionary[mccName]->engine->drawCircleClear(coords, radius, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}	
}

// Draws a filled circle.
void MccEngineGL::drawCircleFilled(std::string mccName, float coords[2], float radius, float color[4])
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Call BaseEngine function.
			m_mccDictionary[mccName]->engine->drawCircleFilled(coords, radius, color);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Adds text to the VBO object.
void MccEngineGL::drawText(std::string mccName, std::string text, float coords[2], float color[4], float scale)
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Render the text.
			m_mccDictionary[mccName]->engine->drawText(text, coords, color, scale);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Draws the demo to the selected mcc.
void MccEngineGL::drawDemo(std::string mccName, unsigned int loopCount) 
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Render the demo drawing.
			m_mccDictionary[mccName]->engine->drawDemo(loopCount);
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Centers the Drawing around (0,0) and scales it to fit into the window.
void MccEngineGL::autoCenter(std::string mccName)
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Render the demo drawing.
			m_mccDictionary[mccName]->engine->autoCenter();
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Centers the Drawing around (0,0) and scales it to fit into the window.
void MccEngineGL::updateBuffers(std::string mccName)
{
	// Check if MCC exists.
	if (isNameInDictionary(mccName))
	{
		try {
			// Render the demo drawing.
			m_mccDictionary[mccName]->engine->updateBuffers();
		}
		catch (const std::exception& e)
		{
			std::cout << "[INTERFACE][ERROR] Invalid parameters caused exception: '" << e.what() << "'.\n\n";
		}
	}
	else
	{
		std::cout << "[INTERFACE][ERROR] '" << mccName << "' is an invalid name.\n\n";
	}
}

// Checks if the given name exists in the MCC dictionary.
bool MccEngineGL::isNameInDictionary(std::string mccName)
{
	return m_mccDictionary.find(mccName) != m_mccDictionary.end();
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
