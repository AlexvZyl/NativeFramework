#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "../Base2DEngineGL/Base2D_Engine.h"

//=============================================================================================================================================//
//  Forward declerations																													   //
//=============================================================================================================================================//

class Component2D;

template<typename VetexType>
class Polygon2D;

//=============================================================================================================================================//
//  Design 2D Engine class.																													   //
//=============================================================================================================================================//
enum designState { COMPONENT_PLACE, ENTITY_SELECT };


class Design2DEngineGL : public Base2DEngineGL
{
public:

	// ------------------ //
	//  V A R I A B L E S //
	// ------------------ //

	designState designerState = ENTITY_SELECT;
	std::vector<std::shared_ptr<Component2D>> m_components;
	std::shared_ptr<Component2D> m_activeComponent;
	Polygon2D<VertexData>* p1;
	Polygon2D<VertexData>* p2;
	Polygon2D<VertexData>* p3;
	unsigned int m_currentEntityID = 0;

	// ------------------------------------------------ //
	//  C O N S T R U C T O R   &   D E S T R U C T O R //
	// ------------------------------------------------ //

	// Constructor
	Design2DEngineGL(GUIState* guiState);
	// Destructor.
	~Design2DEngineGL();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Rendering loop.
	virtual void renderLoop() override;

	// ------- //
	//  A P I  //
	// ------- //

	// --------------------------- //
	//  W I N D O W   E V E N T S  //
	// --------------------------- //

	virtual void resizeEvent(float width, float height) override;

	// --------------------- //
	//  U S E R   I N P U T  //
	// --------------------- //

	// Handling mouse events.
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