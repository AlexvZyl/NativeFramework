//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "mccEngineCore.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions..
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
MccEngineGL::MccEngineGL(stateMachine* states)
	: m_states(states)
{}

// Destructor.
MccEngineGL::~MccEngineGL() 
{
	// Delete engines to prevent memory leaks.
	for (auto engine : m_mccDictionary)
	{
		delete engine.second->engine;
	}
}

// Render loop.
void MccEngineGL::renderActiveEngine() 
{
	// Check if the dict is not empty.
	if (m_mccDictionary.size() != 0)
	{
		// For now render everytihng.
		for (auto engine : m_mccDictionary)
		{
			engine.second->engine->renderLoop();
		}
	}
}

//---------------------------------------------------------------------------------------
// Events.
//---------------------------------------------------------------------------------------

// Resize event for the engine.
void MccEngineGL::resizeEvent(int width, int height) 
{
	// Check if the dict is not empty.	
	if (m_mccDictionary.size() != 0)
	{
		// Run resize event for each engine.
		for (auto engine : m_mccDictionary)
		{
			if (engine.second->resizeEvent)
			{
				engine.second->engine->resizeEventImGUI((int)engine.second->viewportDimentions.x, (int)engine.second->viewportDimentions.y);
				engine.second->resizeEvent = false;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------