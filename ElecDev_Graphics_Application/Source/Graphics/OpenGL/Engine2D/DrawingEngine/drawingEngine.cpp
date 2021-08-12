/*
This is where the drawing enigine is handled.  This is only used to draw elements to the screen.
The interactive engine (the one where elements can be drawn is handled in designEngine).
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "drawingEngine.h"

// Error handler.
#include <ErrorHandler/errorHandler.h>
#include "../Helper/stateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
//  Constructors.
//----------------------------------------------------------------------------------------------------------------------

// Default.
DrawingEngineGL::DrawingEngineGL() {};

// With GLFW window.
DrawingEngineGL::DrawingEngineGL(GLFWwindow* window)
{	
	// Save pointer to GLFW window.
	this->window = window;

	// Create shader.
	std::string shaderFilePath = "Source\\Graphics\\OpenGL\\Shaders\\basicShader.shader";
	GLCall( Shader basicShader(shaderFilePath) );
	GLCall( this->basicShader = basicShader );

	// Assign matrices to shader.
	GLCall( this->basicShader.use() );
	GLCall( this->basicShader.setMat4("worldMatrix", this->modelMatrix) );
	GLCall( this->basicShader.setMat4("projectionMatrix", this->projectionMatrix) );
	GLCall( this->basicShader.setMat4("viewMatrix", this->viewMatrix) );

	std::cout << "[OPENGL SHADER] Shaders compiled.\n";

	// create our geometries
	unsigned int vbo, vao, ebo;

	// create the triangle
	float triangle_vertices[] = {
		0.0f, 0.25f, 0.0f,	// position vertex 1
		1.0f, 0.0f, 0.0f,	 // color vertex 1
		0.25f, -0.25f, 0.0f,  // position vertex 1
		0.0f, 1.0f, 0.0f,	 // color vertex 1
		-0.25f, -0.25f, 0.0f, // position vertex 1
		0.0f, 0.0f, 1.0f,	 // color vertex 1
	};
	unsigned int triangle_indices[] = {
		0, 1, 2 };

	// Assign Buffers.
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	this->VAO = vao;
};

//----------------------------------------------------------------------------------------------------------------------
//  Rendering.
//----------------------------------------------------------------------------------------------------------------------

void DrawingEngineGL::renderLoop()
{
	// Apply translation to shader.
	this->basicShader.setMat4("viewMatrix", this->viewMatrix);

    // rendering our geometries
	GLCall( this->basicShader.use() );
    glBindVertexArray(this->VAO);
    GLCall( glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0) );
    glBindVertexArray(0);

	// Draw temporary border.
	GLCall ( glBegin(GL_LINE_LOOP) );
	glVertex2f(-1.0f,1.0f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(1.0f, -1.0f);
	glVertex2f(-1.0f, -1.0f);
	glEnd();

}

//----------------------------------------------------------------------------------------------------------------------
//  API
//----------------------------------------------------------------------------------------------------------------------

// Adds a line to the VBO object.
void DrawingEngineGL::drawLine()
{
	return;
}

// Adds a circle to the VBO object.
void DrawingEngineGL::drawCircle()
{
	return;
}

// Adds text to the VBO object.
void DrawingEngineGL::drawText()
{
	return;
}

// Displays the new drawing to the screen.
// Required after each new element has been added.
void DrawingEngineGL::display()
{
		
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse events.
//----------------------------------------------------------------------------------------------------------------------

// Event handler for a mouse left click.
void DrawingEngineGL::mousePressLeft(double pixelCoords[2])
{
	// Find current click in world coords.
	glm::vec4 currmousePosVec = this->pixelCoordsToWorldCoords(pixelCoords);
	// Save current mouse pos click.
	this->prevMouseEventWorldCoords[0] = currmousePosVec[0];
	this->prevMouseEventWorldCoords[1] = currmousePosVec[1];
}

// Event handler for a mouse move event.
void DrawingEngineGL::mouseMoveEvent(double pixelCoords[2], int buttonState)
{
	// Check if left mouse is pressed.
	if (buttonState == GLFW_PRESS)
	{
		// Find current mouse position in the world.
		glm::vec4 currMousePosVec = this->pixelCoordsToWorldCoords(pixelCoords);
		// Calculate distance to translate.
		glm::vec3 translateVec({(currMousePosVec[0]-this->prevMouseEventWorldCoords[0]),(currMousePosVec[1]-this->prevMouseEventWorldCoords[1]),0});
		// Translate to the view matrix.
		this->viewMatrix = glm::translate(this->viewMatrix, translateVec);

		// Save mouse click position.
		this->prevMouseEventWorldCoords[0] = currMousePosVec[0];
		this->prevMouseEventWorldCoords[1] = currMousePosVec[1];
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Coordinate systems.
//----------------------------------------------------------------------------------------------------------------------

// Function that takes pixel coordinates as input and return the coordinates in the world.
glm::vec4 DrawingEngineGL::pixelCoordsToWorldCoords(double pixelCoords[2])
{
	// The coordinates on the screen.
	double screenCoords[2];  
	// Find the viewpwort dimensions.
	int viewport[2];
	glfwGetWindowSize(this->window, &viewport[0], &viewport[1]);
	// OpenGL places the (0,0) point in the top left of the screen.  Place it in the bottom left cornder.
	pixelCoords[1] = (double)viewport[1] - pixelCoords[1];

	// Apply the viewport transform the the pixels.
	screenCoords[0] = (pixelCoords[0] - viewport[0] / 2) / (viewport[0] / 2);
	screenCoords[1] = (pixelCoords[1] - viewport[1] / 2) / (viewport[1] / 2);
	// Convert to screen vector.
	glm::vec4 screenVec = { screenCoords[0], screenCoords[1], 0, 1 };

	// Apply MVP matrices.
	glm::mat4 MVPinverse = glm::inverse(this->modelMatrix * this->viewMatrix * this->projectionMatrix);
	glm::vec4 worldVec = MVPinverse * screenVec;

	return worldVec;
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------