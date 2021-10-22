#pragma once

/*
The 3D base engine is going to be a child of the 2D engine so that the same pointer can point to the 3D engine.
Some modifications will have to be applied to the matrices and the mouse handling, but other than that the same principles should apply.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// The base 2D engine.
#include "../Base2DEngine/Base2D_Engine.h"
#include "Peripherals/Camera.h"

// GLM QUaternion.
#include "glm.hpp"

//=============================================================================================================================================//
//  Base 3D Engine Class.																													   //
//=============================================================================================================================================//

// Inherits from the 2D base engine.
class Base3DEngineGL : public BaseEngineGL
{
public:

	//----------------------------------------------------------------------------------------------------------------------
	//  Variables.
	//----------------------------------------------------------------------------------------------------------------------
	
	// 3D scene camera.
	Camera* m_camera;
	// Stores the previous event coords in pixels.  This is required for the camera rotation.
	float m_prevMouseEventPixelCoords[2] = { NULL, NULL };

	//----------------------------------------------------------------------------------------------------------------------
	//  Constructor and Destructor.
	//----------------------------------------------------------------------------------------------------------------------

	// Constructor
	Base3DEngineGL(stateMachine* states);
	// Destructor.
	~Base3DEngineGL();

	//---------------------------------------------------------------------------------------------------------------------
	//  Rendering.
	//---------------------------------------------------------------------------------------------------------------------

	// Rendering loop.
	virtual void renderLoop();
	// Background creation.
	virtual void createBackground();

	//---------------------------------------------------------------------------------------------------------------------
	//  API
	//---------------------------------------------------------------------------------------------------------------------

	virtual void drawQuadFilled3D(float position1[3], float position2[3], float position3[3], float position4[3], float color[4]);
	virtual void drawCuboidFilled(float position1[3], float position2[3], float position3[3], float position4[3], float depth, float color[4]);
	virtual void drawDemo(unsigned int loopCount);

	//---------------------------------------------------------------------------------------------------------------------
	//  Window events.
	//---------------------------------------------------------------------------------------------------------------------

	virtual void resizeEvent(int width, int height);

	//---------------------------------------------------------------------------------------------------------------------
	//  Mouse events.
	//---------------------------------------------------------------------------------------------------------------------

	// Handling mouse events.
	virtual void mousePressLeft(float pixelCoords[2]);
	virtual void mousePressRight(float pixelCoords[2]);
	virtual void mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight);
	virtual void mouseScrollEvent(float pixelCoords[2], float yOffset);

	//--------------------------------------------------------------------------------------------------------------------
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//