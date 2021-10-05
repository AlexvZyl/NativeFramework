#pragma once

/*
This is where the drawing enigine is handled.  This is only used to draw elements to the screen.
The interactive engine (the one where elements can be drawn is handled in DesignEngine).
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------


// Resource loading.
#include <Windows.h>

// OpenGL.
#include <glad/glad.h>
#include <ShaderHandler/shaderHandler.h>
#include <glm.hpp>

// General.
#include <iostream>

// Error handling.
#include <ErrorHandler/errorHandler.h>
#include "../stateMachine.h"

// Buffers.
#include "Peripherals/vertexArrayObject.h"
#include "Peripherals/frameBufferObject.h"

// Text rendering.
#include "Peripherals/textRenderer.h"

// Window.
#include <GLFW/glfw3.h>

//----------------------------------------------------------------------------------------------------------------------
//  Globals.
//----------------------------------------------------------------------------------------------------------------------

const float PI = 3.14159265358979323f;

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
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);		// The matrix that handles the camera movement.
													// viewMatrix = translatinMatrix * rotationMatrix * scalingMatrix;
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);	// The matrix that handles the clipping plane (which part of the world is
													// going to be visible to the screen?
	glm::mat4 m_viewportMatrix = glm::mat4(1.0f);	// The matrix that handles the viewport transform.  Converts screen pixel
													// coordinates to the OpenGL uniform coordinate system.

	// View matrix components.
	glm::mat4 m_scalingMatrix = glm::mat4(1.0f);	// Handles camera scaling.
	glm::mat4 m_translationMatrix = glm::mat4(1.0f);// Handles camera translations.
	glm::mat4 m_rotationMatrix = glm::mat4(1.0f);	// Handles camera rotations.

	// We need matrices to store the base view of the drawing.  This is to fall back to when right clicking, and this has to 
	// be updated with resizing and when auto sizing and scaling funtions are called.
	glm::mat4 m_scalingMatrixBase = glm::mat4(1.0f);	// Stores base matrix for camera scaling.
	glm::mat4 m_translationMatrixBase = glm::mat4(1.0f);// Stores base matrix for camera translation.
	glm::mat4 m_rotationMatrixBase = glm::mat4(1.0f);	// Stores base matrix for camera rotation.

	//---------------------------------------------------------------------------------------------------------------------
	//  Mouse handler variables.
	//---------------------------------------------------------------------------------------------------------------------

	// Saves the previous mouse event coordinates (in world coordinates).
	float m_prevMouseEventWorldCoords[2] = {NULL, NULL};

	//---------------------------------------------------------------------------------------------------------------------
	//  Shaders.
	//---------------------------------------------------------------------------------------------------------------------

	Shader* m_basicShader;		// Renders movable elements without textures.
	Shader* m_staticShader;		// Renders static elements.
	Shader* m_textureShader;	// Renders movable elements with textures.
	Shader* m_textShader;		// Shader that renders text.

	//---------------------------------------------------------------------------------------------------------------------
	//  Buffers.
	//---------------------------------------------------------------------------------------------------------------------

	// Vertex arrays.
	VertexArrayObject* m_linesVAO;				// Lines.
	VertexArrayObject* m_trianglesVAO;			// Triangles.
	VertexArrayObject* m_textureTrianglesVAO;	// Textured Triangles.
	VertexArrayObject* m_textVAO;				// Handles text rendering.
	VertexArrayObject* m_backgroundVAO;			// Background has a seperate VAO since it should not move.
	// Frame Buffer rendering.
	FrameBufferObject* m_frameBuffer;			// FBO to render scene onto.
	// Local Data.								// It is not a good idea to read data from the OpenGL buffers and thus we
												// need to store copies of the buffers on the CPU side.
	std::vector<VertexData> m_verticesCPU;			// A single buffer containing all of the vertices used in the drawing.

	//---------------------------------------------------------------------------------------------------------------------
	//  Settings.
	//---------------------------------------------------------------------------------------------------------------------
	
	float m_scaleRate = 0.3f;				// Determines how much is zoomed with each mouse wheel scroll.
	int m_circleResolution = 25;			// Determines how perfect the circle is (total lines used to draw it).  Very 
											// demanding at high values.

	//---------------------------------------------------------------------------------------------------------------------
	//  State machine.
	//---------------------------------------------------------------------------------------------------------------------

	stateMachine* m_states;					// Contains the current state of the application as well as information
											// that is required from the GUI side for the Graphics side.

	//---------------------------------------------------------------------------------------------------------------------
	//  Misc variables.
	//---------------------------------------------------------------------------------------------------------------------

	float m_imGuiViewportDimensions[2];			// Stores the dimensions of the viewport that the OpenGL context gets drawn
												// to.  
	float m_projectionValues[6];				// Stores the valaues that is used to calculate the projection matrix.

	//---------------------------------------------------------------------------------------------------------------------
	//  Constructor & Destructor.
	//---------------------------------------------------------------------------------------------------------------------

	// Contructor.
	BaseEngineGL(stateMachine* states);
	// Destructor
	~BaseEngineGL();

	//---------------------------------------------------------------------------------------------------------------------
	//  Rendering.
	//---------------------------------------------------------------------------------------------------------------------

	// Variables.
	GLuint m_texture;
	// Texture that contains all of the font information.
	GLuint m_textAtlas;
	// Objects.
	TextRenderer* m_textRenderer;
	// Functions.
	GLuint loadTexture(const std::string& path, bool alpha=false);
	GLuint loadBMPtoGL(int bitmapID);
	void renderLoop();
	// Get the ID to the FBO rendered texture.
	unsigned int getRenderedTexID();

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
	void drawText(std::string text, float coords[2], float color[4], float scale);
	void drawDemo(unsigned int loopCount);											// Draw a demo of the capabilities.
	void autoCenter();

	//---------------------------------------------------------------------------------------------------------------------
	//  Mouse events.
	//---------------------------------------------------------------------------------------------------------------------

	// Handling mouse events.
	void mousePressLeft(float pixelCoords[2]);
	void mousePressRight(float pixelCoords[2]);
	void mouseMoveEvent(float pixelCoords[2], int buttonState);
	void mouseScrollEvent(float pixelCoords[2], int yOffset);

	//---------------------------------------------------------------------------------------------------------------------
	//  Coordinate systems.
	//---------------------------------------------------------------------------------------------------------------------

	// Convert pixel coordinates to world coordinates.
	glm::vec4 pixelCoordsToWorldCoords(float pixelCoords[2]);

	//---------------------------------------------------------------------------------------------------------------------
	//  Window functions.
	//---------------------------------------------------------------------------------------------------------------------

	// Callback that handles the resizing event.
	void resizeEvent(int width, int height);

	//---------------------------------------------------------------------------------------------------------------------
	//  Camera manipulation.
	//---------------------------------------------------------------------------------------------------------------------


};

//-------------------------------------------------------------------------------------------------------------------------
//  EOF.
//-------------------------------------------------------------------------------------------------------------------------