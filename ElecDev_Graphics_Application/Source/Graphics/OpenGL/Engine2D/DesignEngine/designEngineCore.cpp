/*
This is where the drawing enigine is handled.  This is only used to draw elements to the screen.
The interactive engine (the one where elements can be drawn is handled in designEngine).
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Class include.
#include "designEngineCore.h"

//----------------------------------------------------------------------------------------------------------------------
//  Constructor & Destructor.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
DesignEngineGL::DesignEngineGL(stateMachine* states) : BaseEngineGL(states)
{
	std::cout << "[OPENGL][DESIGN ENGINE] Starting...\n";

	float color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
	m_mousePoint = new MousePoint(color, 0.015, 20, &m_projectionMatrix, m_states);

	std::cout << "[OPENGL][DESIGN ENGINE] Done.\n";
}

// Destructor.
DesignEngineGL::~DesignEngineGL() 
{
	delete m_mousePoint;
}

//----------------------------------------------------------------------------------------------------------------------
//  Rendering.
//----------------------------------------------------------------------------------------------------------------------

void DesignEngineGL::renderLoop()
{
	// Call the base engine rendering loop.
	BaseEngineGL::renderLoop();

	// Now add the design engine loop on top.
	m_frameBuffer->bind();

	// Draw the mouse point.
	m_mousePoint->render();

	// Done.
	m_frameBuffer->unbind();
}

//----------------------------------------------------------------------------------------------------------------------
//  Window events.
//----------------------------------------------------------------------------------------------------------------------

void DesignEngineGL::resizeEvent(int width, int height) 
{
	// Call base engine resize event.
	BaseEngineGL::resizeEvent(width, height);

	// Now call resizing required for design engine.
	m_mousePoint->updateProjection();
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------