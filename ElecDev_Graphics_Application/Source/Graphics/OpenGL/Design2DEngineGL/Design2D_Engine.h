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
template<typename VetexType>
class Circle;
class Circuit;
class Cable;
class Port;

//=============================================================================================================================================//
//  Design 2D Engine class.																													   //
//=============================================================================================================================================//

// Design engine states.
enum designState { COMPONENT_PLACE, ENTITY_SELECT, COMPONENT_MOVE, CABLE_PLACE};

class Design2DEngineGL : public Base2DEngineGL
{
public:

	// ------------------ //
	//  V A R I A B L E S //
	// ------------------ //

	designState designerState = ENTITY_SELECT;
	//std::vector<std::shared_ptr<Component2D>> m_components;
	std::shared_ptr<Component2D> m_activeComponent;
	std::shared_ptr<Cable> m_activeCable;
	std::shared_ptr<Circuit> m_circuit;
	std::unique_ptr<Circle<VertexDataCircle>> p1;
	std::unique_ptr<Circle<VertexDataCircle>> p2;
	std::unique_ptr<Circle<VertexDataCircle>> p3;
	unsigned int m_currentEntityID = 0;

	// ------------------------------------------------ //
	//  C O N S T R U C T O R   &   D E S T R U C T O R //
	// ------------------------------------------------ //

	// Constructor
	Design2DEngineGL(GUIState* guiState, std::string contextName);
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

	//Helper functions
	virtual void setActiveComponent(unsigned eID);
	virtual void setActiveCable(unsigned eID);
	virtual void ComponentPlaceMode(float screenCoords[2]);
	virtual Port* getPort(unsigned eID);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//