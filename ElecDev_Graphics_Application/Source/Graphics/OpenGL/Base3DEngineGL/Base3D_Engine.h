#pragma once

/*
The 3D base engine is going to be a child of the 2D engine so that the same pointer can point to the 3D engine.
Some modifications will have to be applied to the matrices and the mouse handling, but other than that the same principles should apply.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// The base 2D engine.
#include "../CoreGL/EngineCoreGL.h"
#include "Peripherals/CameraGL.h"

//=============================================================================================================================================//
//  Base 3D Engine Class.																													   //
//=============================================================================================================================================//

// Inherits from the 2D base engine.
class Base3DEngineGL : public EngineCoreGL
{
public:

	// ------------------- //
	//  V A R I A B L E S  //
	// ------------------- //
	
	// 3D scene camera.
	Camera* m_camera;
		
	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Constructor
	Base3DEngineGL(GUIState* guiState);
	// Destructor.
	~Base3DEngineGL();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Rendering loop.
	virtual void renderLoop();

	// ------- //
	//  A P I  //
	// ------- //

	virtual void drawQuadFilled3D(float position1[3], float position2[3], float position3[3], float position4[3], float color[4]);
	virtual void drawCuboidFilled(float position1[3], float position2[3], float position3[3], float position4[3], float depth, float color[4]);
	virtual void drawDemo(unsigned int loopCount);
	virtual void autoCenter();

	// --------------------------- //
	//  W I N D O W   E V E N T S  //
	// --------------------------- //

	virtual void resizeEvent(float width, float height);

	// ------------------------- //
	//  M O U S E   E V E N T S  //
	// ------------------------- //

	// Handling mouse events.
	virtual void mousePressLeft(float pixelCoords[2]);
	virtual void mousePressRight(float pixelCoords[2]);
	virtual void mousePressMiddle(float pixelCoords[2]);
	virtual void mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight);
	virtual void mouseScrollEvent(float pixelCoords[2], float yOffset);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//