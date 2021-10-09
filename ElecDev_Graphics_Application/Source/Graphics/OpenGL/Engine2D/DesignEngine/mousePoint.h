#pragma once

/* 
This code contains the class that handles the mouse point that snaps to components on the screen.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <glad/glad.h>
#include "../BaseEngine/Peripherals/vertexArrayObject.h"
#include <GLFW/glfw3.h>

//----------------------------------------------------------------------------------------------------------------------
//  Mouse point class.
//----------------------------------------------------------------------------------------------------------------------

class MousePoint 
{

private:
	// VAO that contains the mouse point.
	VertexArrayObject* m_VAO;
	// Mouse point color.
	float m_color[4];
	// Mouse point size.
	float m_size;

public:

	//Constructor.
	MousePoint(float color[4], float size);
	// Destructor.
	~MousePoint();

	// Update the mouse point and render to the screen.
	void render(float coords[2]);

};

// Should be moved to design engine.
// Update and draw mouse point.
//m_staticShader->bind();
//double coords[2];
//glfwGetCursorPos(m_window, &coords[0], &coords[1]);
//glm::vec4 mouseWorldCoords = pixelCoordsToWorldCoords(coords);
//float drawCoords[2] = { mouseWorldCoords[0],  mouseWorldCoords[1] };
//m_mousePoint->render(drawCoords);

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------