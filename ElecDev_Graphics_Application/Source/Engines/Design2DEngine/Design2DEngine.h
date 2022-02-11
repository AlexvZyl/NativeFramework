#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/Base2DEngine/Base2DEngine.h"

//=============================================================================================================================================//
//  Forward declerations																													   //
//=============================================================================================================================================//

class Circle;
class Polygon2D;
class Component2D;
class Circuit;
class Cable;
class Port;

//=============================================================================================================================================//
//  Design 2D Engine class.																													   //
//=============================================================================================================================================//

// Design engine states.
enum designState 
{ 
	COMPONENT_PLACE, 
	ENTITY_SELECT, 
	COMPONENT_MOVE, 
	CABLE_PLACE
};

class Design2DEngine : public Base2DEngine
{
public:

	// ------------------ //
	//  V A R I A B L E S //
	// ------------------ //

	designState designerState = ENTITY_SELECT;
	std::shared_ptr<Component2D> m_activeComponent;
	std::shared_ptr<Cable> m_activeCable;
	std::shared_ptr<Circuit> m_circuit;
	glm::vec2 m_lastDragPos = {0.f, 0.f};
	unsigned int m_currentEntityID = 0;
	Port* m_hoveredPort = nullptr;
	unsigned m_hoveredID;

	// ------------------------------------------------ //
	//  C O N S T R U C T O R   &   D E S T R U C T O R //
	// ------------------------------------------------ //

	// Constructor
	Design2DEngine();

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Mouse events.
	void onMouseButtonEvent(MouseButtonEvent& event) override;
	void onMouseMoveEvent(MouseMoveEvent& event) override;
	void onMouseScrollEvent(MouseScrollEvent& event) override;
	// Key events.
	void onKeyEvent(KeyEvent& event) override;
	
	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	void setActiveComponent(unsigned eID);
	void setActiveCable(unsigned eID);
	void ComponentPlaceMode(glm::vec2 screenCoords);
	void deleteActiveComponent();
	void deleteActiveCable();
	Port* getPort(unsigned eID);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//