/*
This is where the drawing enigine mouse events are handled.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "mccEngineCore.h"

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Press event.
//----------------------------------------------------------------------------------------------------------------------

void MccEngineGL::mousePressEvent(int button, int action) 
{
	// Check if MCC dict is empty or inactive.
	if ((m_mccDictionary.size() != 0) && (isNameInDictionary(m_activeMCC)))
	{
		// Find cursos position.
		float mousePos[2] = { m_mccDictionary[m_activeMCC]->mouseCoords.x , m_mccDictionary[m_activeMCC]->mouseCoords.y };
		// Check if left press.
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			// Call active engine.
			m_mccDictionary[m_activeMCC]->engine->mousePressLeft(mousePos);
		}
		// Check if right press.
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			// Call active engine.
			m_mccDictionary[m_activeMCC]->engine->mousePressRight(mousePos);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Move event.
//----------------------------------------------------------------------------------------------------------------------

void MccEngineGL::mouseMoveEvent(int buttonState)
{
	// Check if MCC dict is empty or inactive.
	if ((m_mccDictionary.size() != 0) && (isNameInDictionary(m_activeMCC)))
	{
		// Find cursos position.
		float mousePos[2] = { m_mccDictionary[m_activeMCC]->mouseCoords.x , m_mccDictionary[m_activeMCC]->mouseCoords.y };
		m_mccDictionary[m_activeMCC]->engine->mouseMoveEvent(mousePos, buttonState);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Scroll event.
//----------------------------------------------------------------------------------------------------------------------

void MccEngineGL::mouseScrollEvent(float yOffset)
{
	// Check if MCC dict is empty or inactive.
	if ((m_mccDictionary.size() != 0) && (isNameInDictionary(m_activeMCC)))
	{
		// Find cursos position.
		float mousePos[2] = { m_mccDictionary[m_activeMCC]->mouseCoords.x , m_mccDictionary[m_activeMCC]->mouseCoords.y };
		m_mccDictionary[m_activeMCC]->engine->mouseScrollEvent(mousePos, (int)yOffset);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------