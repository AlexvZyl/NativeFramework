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

	// Find the viewpwort dimensions.
	int viewport[2];
	glfwGetWindowSize(this->window, &viewport[0], &viewport[1]);

	// Matrices.
	this->projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	this->modelMatrix = glm::mat4(1.0f);
	this->viewMatrix = glm::mat4(1.0f);
	this->viewportMatrix = glm::mat4(1.0f);
	this->scalingMatrix = glm::mat4(1.0f);
	this->translationMatrix = glm::mat4(1.0f);
	this->rotationMatrix = glm::mat4(1.0f);

	// Assign matrices to shader.
	GLCall( this->basicShader.use() );
	GLCall( this->basicShader.setMat4("worldMatrix", this->modelMatrix) );
	GLCall( this->basicShader.setMat4("projectionMatrix", this->projectionMatrix) );
	GLCall( this->basicShader.setMat4("viewMatrix", this->viewMatrix) );

	// Print success message.
	std::cout << "[OPENGL SHADER] Shaders compiled.\n";

	// Mouse event variables.
	this->scaleRate = 0.1;

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
	// Account for pixel offset.
	float viewportOffset[2] = { viewport[0] + 1, viewport[1] + 1 };
	// OpenGL places the (0,0) point in the top left of the screen.  Place it in the bottom left cornder.
	double pixelCoordsTemp[2] = { pixelCoords[0] + 1, (double)viewport[1] - pixelCoords[1] + 1};
	std::cout << viewportOffset[0] << " , " << viewportOffset[1] << std::endl;
	
	// Apply the viewport transform the the pixels.
	screenCoords[0] = (pixelCoordsTemp[0] - viewportOffset[0] / 2) / (viewportOffset[0] / 2);
	screenCoords[1] = (pixelCoordsTemp[1] - viewportOffset[1] / 2) / (viewportOffset[1] / 2);
	// Convert to screen vector.
	glm::vec4 screenVec = { screenCoords[0], screenCoords[1], 0, 1 };

	// Apply MVP matrices.
	this->viewMatrix = this->scalingMatrix * this->rotationMatrix * this->translationMatrix;
	glm::mat4 MVPinverse = glm::inverse(this->modelMatrix * this->viewMatrix * this->projectionMatrix);
	glm::vec4 worldVec = screenVec * MVPinverse ;

	return worldVec;
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------