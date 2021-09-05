/*
This code contains the class that handles the mouse point that snaps to components on the screen.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "mousePoint.h"

//----------------------------------------------------------------------------------------------------------------------
//  Mouse methods.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
MousePoint::MousePoint(GLFWwindow* window, float color[4], float size)
	: m_color{color[0], color[1], color[2], color[3]}, m_size(size)

{
	m_VAO = new VertexArrayObject(DrawType::QUAD_FILLED, 4);
}

// Destructor.
MousePoint::~MousePoint() 
{
	delete m_VAO;
}

void MousePoint::render(float coords[2])
{	
	// Create vertices.
	VertexData v1(
		coords[0] + m_size,
		coords[1] + m_size,
		0,
		m_color[0], m_color[1], m_color[2], m_color[3]);
	VertexData v2(
		coords[0] + m_size,
		coords[1] - m_size,
		0,
		m_color[0], m_color[1], m_color[2], m_color[3]);
	VertexData v3(
		coords[0] - m_size,
		coords[1] - m_size,
		0,
		m_color[0], m_color[1], m_color[2], m_color[3]);
	VertexData v4(
		coords[0] - m_size,
		coords[1] + m_size,
		0,
		m_color[0], m_color[1], m_color[2], m_color[3]);

	// Reset pointer.
	m_VAO->m_bufferPtr = 0;
	// Write the data.
	m_VAO->writeData(v1, v2, v3, v4);
	m_VAO->render();
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
