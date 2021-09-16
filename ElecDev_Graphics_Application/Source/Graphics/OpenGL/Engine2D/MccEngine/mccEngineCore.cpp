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
		delete engine.second;
	}
}

// Render loop.
void MccEngineGL::renderActiveEngine() 
{
	// Check if the dict is not empty.
	if (m_mccDictionary.size() != 0)
	{
		// Render active engine.
		/*m_mccDictionary[m_activeMCC]->renderLoop();*/

		// For now render everytihng.
		for (auto engine : m_mccDictionary)
		{
			engine.second->engine->renderLoop();
		}
	}
}

// Add MCC to the dictS.
void MccEngineGL::addMcc(std::string mccName) 
{
	// Add to dictionary.
	m_mccDictionary.insert({ mccName, new MccStruct(m_states) });
	// Set MCC name.
	m_activeMCC = mccName;
}

// Remove an MCC from the dict.
void MccEngineGL::removeMCC(std::string mccName) 
{
	delete m_mccDictionary[mccName];
	// Remove by key.
	m_mccDictionary.erase(mccName);
}

//---------------------------------------------------------------------------------------
// Events.
//---------------------------------------------------------------------------------------

// Resize event for the engine.
void MccEngineGL::resizeEvent(int width, int height) 
{
	// Check if the dict is not empty.

	std::cout << m_mccDictionary.size() << std::endl;
	
	if (m_mccDictionary.size() != 0)
	{
		// Run resize event for each engine.
		for (auto engine : m_mccDictionary)
		{
			engine.second->engine->resizeEventImGUI(engine.second->viewportDimentions.x, engine.second->viewportDimentions.y);
		}
	}

	// Reset bool.
	m_states->renderResizeEvent = false;

}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------