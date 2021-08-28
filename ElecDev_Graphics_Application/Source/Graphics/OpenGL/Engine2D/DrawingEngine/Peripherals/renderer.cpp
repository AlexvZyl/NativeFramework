/*
This is where the drawing enigine rendering is handled.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "Engine2D/DrawingEngine/drawingEngine.h"

// Error handler.
#include <ErrorHandler/errorHandler.h>
#include "../Helper/stateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

void DrawingEngineGL::renderLoop()
{
	// Apply translation to shader.
	this->viewMatrix = this->scalingMatrix * this->rotationMatrix * this->translationMatrix;
	this->basicShader.setMat4("viewMatrix", this->viewMatrix);

	// rendering our geometries
	GLCall(this->basicShader.use());
	GLCall(glBindVertexArray(this->VAO));
	GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));

	// Draw temporary border.
	GLCall(glBegin(GL_LINE_LOOP));
	glVertex2f(-1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(1.0f, -1.0f);
	glVertex2f(-1.0f, -1.0f);
	glEnd();

}