 #pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <glm.hpp>				
#include <string>
#include <memory>

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

class Event;
class MouseButtonEvent;
class MouseMoveEvent;
class MouseScrollEvent;
class KeyEvent;
class WindowEvent;
class LayerEvent;
class Scene;

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
	EngineCore() = default;

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

	// Pass the event to the engine.
	virtual void onEvent(Event& event);

	// Mouse events.
	inline virtual void onMouseButtonEvent(MouseButtonEvent& event) = 0;
	inline virtual void onMouseMoveEvent(MouseMoveEvent& event) = 0;
	inline virtual void onMouseScrollEvent(MouseScrollEvent& event) = 0;

	// Key events.
	inline virtual void onKeyEvent(KeyEvent& event) = 0;

	// Window events.
	virtual void onWindowResizeEvent(WindowEvent& event);

	// Layer events.
	virtual void onFocusEvent(LayerEvent& event);
	virtual void onDefocusEvent(LayerEvent& event);
	virtual void onHoverEvent(LayerEvent& event);
	virtual void onDehoverEvent(LayerEvent& event);

	// Stores the previous mouse event information.
	float m_prevMouseEventPixelCoords[2] = { NULL, NULL };		
	// Stores the vector that goes into the world.
	glm::vec3 m_prevMouseEventWorldVec = { NULL, NULL, NULL };	

	// ------------- //
	//  S T A T E S  //
	// ------------- //

	// Is the mouse hovering the engine?
	bool m_isHovered = false;
	// Is the engine focused?
	bool m_isFocused = false;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//