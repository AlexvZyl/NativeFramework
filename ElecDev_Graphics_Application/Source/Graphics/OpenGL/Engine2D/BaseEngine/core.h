#pragma once

/*
This is where the drawing enigine is handled.  This is only used to draw elements to the screen.
The interactive engine (the one where elements can be drawn is handled in designEngine).
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// OpenGL.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderHandler/shaderHandler.h>
#include <glm.hpp>

// General.
#include <iostream>

// Error handling.
#include <ErrorHandler/errorHandler.h>
#include "../Helper/stateMachine.h"

// Buffers.
#include "Peripherals/vertexArrayObject.h"

// Text rendering.
#include <ft2build.h>
#include FT_FREETYPE_H  

//----------------------------------------------------------------------------------------------------------------------
//  Globals.
//----------------------------------------------------------------------------------------------------------------------

const float PI = 3.14159265358979323;

//----------------------------------------------------------------------------------------------------------------------
//  The drawingEngine class.
//----------------------------------------------------------------------------------------------------------------------

class BaseEngineGL
{
public:

	//---------------------------------------------------------------------------------------------------------------------
	// MPV Matrices.
	//---------------------------------------------------------------------------------------------------------------------
	
	// MVP Matrices.
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);		// The model matrix that places the object in the world.  Is going to be 
													// kept an identity matrix for now.
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);			// The matrix that handles the camera movement.
													// viewMatrix = translatinMatrix * rotationMatrix * scalingMatrix;
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);	// The matrix that handles the clipping plane (which part of the world is
													// going to be visible to the screen?
	glm::mat4 m_viewportMatrix = glm::mat4(1.0f);		// The matrix that handles the viewport transform.  Converts screen pixel
													// coordinates to the OpenGL uniform coordinate system.

	// View matrix components.
	glm::mat4 m_scalingMatrix = glm::mat4(1.0f);		// Handles camera scaling.
	glm::mat4 m_translationMatrix = glm::mat4(1.0f);	// Handles camera translations.
	glm::mat4 m_rotationMatrix = glm::mat4(1.0f);		// Handles camera rotations.

	//---------------------------------------------------------------------------------------------------------------------
	//  Mouse handler variables.
	//---------------------------------------------------------------------------------------------------------------------

	// Saves the previous mouse event coordinates (in world coordinates).
	double m_prevMouseEventWorldCoords[2] = {NULL, NULL};

	//---------------------------------------------------------------------------------------------------------------------
	//  Shaders.
	//---------------------------------------------------------------------------------------------------------------------

	Shader* m_basicShader;
	Shader* m_staticShader;
	Shader* m_textureShader;

	//---------------------------------------------------------------------------------------------------------------------
	//  Buffers.
	//---------------------------------------------------------------------------------------------------------------------

	// Lines.
	VertexArrayObject* m_linesVAO;
	// Triangles.
	VertexArrayObject* m_trianglesVAO;
	// Textured Triangles.
	VertexArrayObject* m_texTrianglesVAO;
	// Background has a seperate VAO since it should not move.
	VertexArrayObject* m_backgroundVAO;

	//---------------------------------------------------------------------------------------------------------------------
	//  Settings.
	//---------------------------------------------------------------------------------------------------------------------
	
	float m_scaleRate = 0.3;
	unsigned int m_circleResolution = 35;

	//---------------------------------------------------------------------------------------------------------------------
	//  Misc variables.
	//---------------------------------------------------------------------------------------------------------------------

	GLFWwindow* m_window;
	float m_viewportDimensions[2];
	float m_projectionValues[6];

	//---------------------------------------------------------------------------------------------------------------------
	//  Constructor & Destructor.
	//---------------------------------------------------------------------------------------------------------------------

	// With GLFW window.
	BaseEngineGL(GLFWwindow* window);
	~BaseEngineGL();
		
	//---------------------------------------------------------------------------------------------------------------------
	//  Rendering.
	//---------------------------------------------------------------------------------------------------------------------

	GLuint m_texture;
	void renderLoop();
	GLuint loadTexture(const std::string& path);

	//---------------------------------------------------------------------------------------------------------------------
	//  API
	//---------------------------------------------------------------------------------------------------------------------

	// Functions supported by the base engine.
	void drawLine(float position1[2], float position2[2], float color[4]);
	void drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4]);
	void drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4]);
	void drawQuadClear(float position[2], float width, float height, float color[4]);
	void drawQuadFilled(float position[2], float width, float height, float color[4]);
	void drawCircleClear(float position[2], float radius, float color[4]);
	void drawCircleFilled(float position[2], float radius, float color[4]);
	void drawText();
	void display();

	//---------------------------------------------------------------------------------------------------------------------
	//  Mouse events.
	//---------------------------------------------------------------------------------------------------------------------

	// Handling mouse events.
	void mousePressLeft(double pixelCoords[2]);
	void mousePressRight(double pixelCoords[2]);
	void mouseMoveEvent(double pixelCoords[2], int buttonState);
	void mouseScrollEvent(double pixelCoords[2], int yOffset);

	//---------------------------------------------------------------------------------------------------------------------
	//  Coordinate systems.
	//---------------------------------------------------------------------------------------------------------------------

	// Convert pixel coordinates to world coordinates.
	glm::vec4 pixelCoordsToWorldCoords(double pixelCoords[2]);

	//---------------------------------------------------------------------------------------------------------------------
	//  Window functions.
	//---------------------------------------------------------------------------------------------------------------------

	// Callback that handles the resizing event.
	void resizeEvent(int width, int height);

};

//-------------------------------------------------------------------------------------------------------------------------
//  EOF.
//-------------------------------------------------------------------------------------------------------------------------