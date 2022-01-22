#pragma once

/*

*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/Core/EngineCore.h"

//=============================================================================================================================================//
//  Base 2D Engine Class.																													   //
//=============================================================================================================================================//

class Base2DEngine : public EngineCore
{

public:

	// --------------------------------- //
	//  E N G I N E   V A R I A B L E S  //
	// --------------------------------- //

	float m_scaleRate = 0.3f;		// Determines how much is zoomed with each mouse wheel scroll.
	float m_projectionValues[6];	// Stores the valaues that is used to calculate the projection matrix.

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Contructor.
	Base2DEngine(GUIState* guiState, std::string contextName);

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Testing texture.
	unsigned int m_texture=0;

	// ------------ //
	//  2D   A P I  //
	// ------------ //

	virtual void drawLine(float position1[2], float position2[2], float color[4]);
	virtual void drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4]);
	virtual void drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4]);
	virtual void drawQuadClear(float position[2], float width, float height, float color[4]);
	virtual void drawQuadFilled(float position[2], float width, float height, float color[4]);
	virtual void drawCircleClear(float position[2], float radius, float color[4]);
	virtual void drawCircleFilled(float position[2], float radius, float color[4]);
	virtual void drawText(std::string text, float coords[2], float color[4], float scale, std::string align);
	virtual void drawDemo(unsigned int loopCount) override;
	virtual void autoCenter() override;

	// --------------------- //
	//  U S E R   I N P U T  //
	// --------------------- //

	virtual void mousePressLeft(float pixelCoords[2]) override;
	virtual void mousePressRight(float pixelCoords[2]) override;
	virtual void mousePressMiddle(float pixelCoords[2]) override;
	virtual void mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle) override;
	virtual void mouseScrollEvent(float pixelCoords[2], float yOffset) override;
	virtual void keyEvent(int key, int action) override;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//