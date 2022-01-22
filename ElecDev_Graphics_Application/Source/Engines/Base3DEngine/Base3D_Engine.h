#pragma once

/*
The 3D base engine is going to be a child of the 2D engine so that the same pointer can point to the 3D engine.
Some modifications will have to be applied to the matrices and the mouse handling, but other than that the same principles should apply.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// The base 2D engine.
#include "Engines/Core/EngineCore.h"

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

class Camera;

//=============================================================================================================================================//
//  Base 3D Engine Class.																													   //
//=============================================================================================================================================//

// Inherits from the 2D base engine.
class Base3DEngine : public EngineCore
{
public:
		
	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Constructor
	Base3DEngine(GUIState* guiState, std::string contextName);

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

	// ----------------------- //
	//  U S E R   I N P U T S  //
	// ----------------------- //

	// Handling mouse events.
	virtual void mousePressLeft(float pixelCoords[2]);
	virtual void mousePressRight(float pixelCoords[2]);
	virtual void mousePressMiddle(float pixelCoords[2]);
	virtual void mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle);
	virtual void mouseScrollEvent(float pixelCoords[2], float yOffset);
	virtual void keyEvent(int key, int action);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//