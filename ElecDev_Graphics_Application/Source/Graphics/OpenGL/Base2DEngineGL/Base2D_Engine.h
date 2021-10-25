#pragma once

/*
Child of EngineCoreGL.  Uses the base OpenGL functionality and implements 2D controls and API.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Core OpenGL functionality.
#include "../CoreGL/EngineCoreGL.h"

//=============================================================================================================================================//
//  Base 2D Engine Class.																													   //
//=============================================================================================================================================//

class Base2DEngineGL : public EngineCoreGL
{

public:

	// ------------------------------- //
	//  E N G I N E   S E T T I N G S  //
	// ------------------------------- //

	float m_scaleRate = 0.3f;		// Determines how much is zoomed with each mouse wheel scroll.
	float m_projectionValues[6];	// Stores the valaues that is used to calculate the projection matrix.

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Contructor.
	Base2DEngineGL(stateMachine* states);
	// Destructor
	~Base2DEngineGL();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Testing texture.
	GLuint m_texture=0;
	// Rendering loop.
	virtual void renderLoop();
	// Get the ID to the FBO rendered texture.
	virtual unsigned int getRenderTexture();
	virtual void updateGPU();
	// Callback that handles the resizing event.
	virtual void resizeEvent(float width, float height);

	// ------------ //
	//  2D   A P I  //
	// ------------ //

	// Functions supported by the base engine.
	virtual void drawLine(float position1[2], float position2[2], float color[4]);
	virtual void drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4]);
	virtual void drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4]);
	virtual void drawQuadClear(float position[2], float width, float height, float color[4]);
	virtual void drawQuadFilled(float position[2], float width, float height, float color[4]);
	virtual void drawCircleClear(float position[2], float radius, float color[4]);
	virtual void drawCircleFilled(float position[2], float radius, float color[4]);
	virtual void drawText(std::string text, float coords[2], float color[4], float scale, std::string align);
	virtual void drawDemo(unsigned int loopCount);								
	virtual void autoCenter();

	// ------------------------- //
	//  M O u S E   E V E N T S  //
	// ------------------------- //

	// Handling mouse events.
	virtual void mousePressLeft(float pixelCoords[2]);
	virtual void mousePressRight(float pixelCoords[2]);
	virtual void mousePressMiddle(float pixelCoords[2]);
	virtual void mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight);
	virtual void mouseScrollEvent(float pixelCoords[2], float yOffset);

	// ----------------------- //
	//  C O O R D I n A T E S  //
	// ----------------------- //

	// Convert pixel coordinates to world coordinates.
	glm::vec4 pixelCoordsToWorldCoords(float pixelCoords[2]);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//