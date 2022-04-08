 #pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "glm/glm.hpp"
#include <string>
#include <memory>
#include "OpenGL/SceneGL.h"

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

class Event;
class MouseButtonEvent;
class MouseMoveEvent;
class MouseScrollEvent;
class MouseDragEvent;
class KeyEvent;
class WindowEvent;
class NotifyEvent;
class FileDropEvent;
class Layer;

//=============================================================================================================================================//
//  Variables and constants.																												   //
//=============================================================================================================================================//

const float PI = 3.14159265358979323f;

//=============================================================================================================================================//
//  EngineCore Class.																														   //
//=============================================================================================================================================//

class EngineCore
{
public:

	// Constructor
	inline EngineCore() = default;
	// Destructor.
	virtual ~EngineCore();

	// ---------- //
	//  S C E N E //
	// ---------- //
	
	std::unique_ptr<Scene> m_scene = nullptr;

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Main loop where the rendering happens.
	virtual void onRender();

	// Autocenters the current scene.
	inline virtual void autoCenter() {};

	// Draws elements to the screen.  Used for debugging and benchmarking.
	// Should also be implemented to showcase the capabilities of the engine.
	inline virtual void drawDemo(unsigned loopCount) {};

	// Returns the FBO texture ID that can be rendered.
	unsigned int getRenderTexture();

	// Calculates the delta time and returns it.	
	// This is used to ensure constant movement and sensitivity
	// regardless of the framerate.
	float deltaTime();

	// Returns the ID of the entity in the pixel coords.
	unsigned int getEntityID(glm::vec2& pixelCoords);

	// The difference between the last 2 rendred frame.
	float m_deltaTime = 0.0f;	
	// The time of the previous frame.
	float m_lastFrame = 0.0f;	

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	void onEvent(Event& event);
	inline virtual void onMouseButtonEvent(MouseButtonEvent& event) {}
	inline virtual void onMouseMoveEvent(MouseMoveEvent& event) {}
	inline virtual void onMouseScrollEvent(MouseScrollEvent& event) {}
	inline virtual void onMouseDragEvent(MouseDragEvent& event) {}
	inline virtual void onKeyEvent(KeyEvent& event) {};
	inline virtual void onWindowResizeEvent(WindowEvent& event) {}
	inline virtual void onNotifyEvent(NotifyEvent& event) {}
	inline virtual void onFocusEvent(NotifyEvent& event) {}
	inline virtual void onDefocusEvent(NotifyEvent& event) {}
	inline virtual void onHoverEvent(NotifyEvent& event) {}
	inline virtual void onDehoverEvent(NotifyEvent& event) {}
	inline virtual void onFileDropEvent(FileDropEvent& event) {};

	// --------- //
	//  D A T A  //
	// --------- //

	// Stores the previous mouse event information.
	glm::vec2 m_prevMouseEventPixelCoords = { NULL, NULL };		
	// Stores the vector that goes into the world.
	glm::vec3 m_prevMouseEventWorldVec = { NULL, NULL, NULL };	
	// Set the name elements of the engine.
	inline virtual void setName(std::string& name) {}

	// The layer the engine belongs to.
	Layer* m_layer;

	// ------------- //
	//  S T A T E S  //
	// ------------- //

	// Is the mouse hovering the engine?
	bool m_isHovered = false;
	// Is the engine focused?
	bool m_isFocused = false;
	
	inline bool hasDesignPalette() 
	{ 
		return m_hasDesignPalette; 
	}

	inline virtual void renderDesignPalette() {};

	glm::vec2 getNearestGridVertex(const glm::vec2& coords);

	// Get muouse position helpers.
	glm::vec2 getMouseLocalPosition();
	glm::vec2 getMouseGlobalPosition();

	// Set content region pos.
	void setContentRegionPos(const glm::vec2& pos);

	// The graphics window size.
	glm::vec2 m_contentRegionSize = { 0,0 };
	// The graphics window position in the window.
	glm::vec2 m_contentRegionPos = { 0, 0 };
	// Convert screen pixel distance to distance in the world.
	glm::vec2 pixelDistanceToWorldDistance(const glm::vec2& distance);
	glm::vec3 pixelCoordsToWorldCoords(const glm::vec2& coords);
	glm::vec3 pixelCoordsToCameraCoords(const glm::vec2& coords);

protected:

	inline void enableDesignPalette() 
	{
		m_hasDesignPalette = true;
	}
	inline void disableDesignPalette()
	{
		m_hasDesignPalette = false;
	}

private:

	// Does the engine have a design palette?
	bool m_hasDesignPalette = false;

	// Handlers that are always called on the events.
	// Prevents children from overriding certain behaviour.
	virtual void onNotifyEventForce(NotifyEvent& event);
	virtual void onMouseMoveEventForce(MouseMoveEvent& event);
	virtual void onFocusEventForce(NotifyEvent& event);
	virtual void onDefocusEventForce(NotifyEvent& event);
	virtual void onHoverEventForce(NotifyEvent& event);
	virtual void onDehoverEventForce(NotifyEvent& event);
	virtual void onWindowResizeEventForce(WindowEvent& event);

};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
