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

//---------------------------------------------------------------------------------------------------------------------
//  Testing.
//----------------------------------------------------------------------------------------------------------------------

DrawingEngineGL::DrawingEngineGL()
{	
	// Create shader.
	std::string shaderFilePath = "Source\\Graphics\\OpenGL\\Shaders\\basicShader.shader";
	GLCall( Shader basicShader(shaderFilePath) );
	GLCall( this->basicShader = basicShader );

	// Assign matrices to shader.
	GLCall( this->basicShader.use() );
	GLCall( this->basicShader.setMat4("worldMatrix", this->modelMatrix) );
	GLCall( this->basicShader.setMat4("projectionMatrix", this->projectionMatrix) );
	GLCall( this->basicShader.setMat4("viewMatrix", this->viewMatrix) );

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

void DrawingEngineGL::renderLoop()
{
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

//---------------------------------------------------------------------------------------------------------------------
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



//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------