#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/Base2DEngine/Base2DEngine.h"
#include <iostream>
#include "yaml-cpp/yaml.h"
#include <filesystem>

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

	// Constructor
	Design2DEngine();
	// Destructor.
	virtual ~Design2DEngine() override;
	
	// ------------------ //
	//  V A R I A B L E S //
	// ------------------ //

	designState designerState = ENTITY_SELECT;
	std::shared_ptr<Component2D> m_activeComponent;
	std::shared_ptr<Cable> m_activeCable;
	unsigned m_activeVertexIdx = -1;
	std::shared_ptr<Circuit> m_circuit;
	glm::vec2 m_lastDragPos = {0.f, 0.f};
	unsigned int m_currentEntityID = 0;
	Port* m_hoveredPort = nullptr;
	unsigned m_hoveredID;

	float clickTol = 0.01f;

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Mouse events.
	virtual void onMouseButtonEvent(MouseButtonEvent& event) override;
	virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
	virtual void onMouseScrollEvent(MouseScrollEvent& event) override;
	virtual void onMouseDragEvent(MouseDragEvent& event) override;
	// Key events.
	virtual void onKeyEvent(KeyEvent& event) override;

	// File events.
	virtual void onFileDropEvent(FileDropEvent& event) override;

	//Notify event.
	virtual void onNotifyEvent(NotifyEvent& event) override;
	
	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	void setActiveComponent(unsigned eID);
	void setActiveCable(unsigned eID);
	void ComponentPlaceMode(glm::vec2 screenCoords);
	void ComponentPlaceMode();
	void deleteActiveComponent();
	void deleteActiveCable();
	Port* getPort(unsigned eID);
	virtual void setName(const std::string& name) override;
	void createCircuit(const std::filesystem::path& path);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//