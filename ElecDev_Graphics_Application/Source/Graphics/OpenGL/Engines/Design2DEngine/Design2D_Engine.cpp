/*
This is where the drawing enigine is handled.  This is only used to draw elements to the screen.
The interactive engine (the one where elements can be drawn is handled in designEngine).
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Class include.
#include "Design2D_Engine.h"

//----------------------------------------------------------------------------------------------------------------------
//  Constructor & Destructor.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
DesignEngineGL::DesignEngineGL(stateMachine* states) : BaseEngineGL(states)
{
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine starting...";

	float color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
	m_mousePoint = new MousePoint(color, 0.015f, 20, &m_projectionMatrix, m_states);

	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine done.";
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