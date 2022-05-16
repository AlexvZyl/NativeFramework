 #pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "glm/glm.hpp"
#include <string>
#include <memory>

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
class YamlNodeDropEvent;
class LumenWindow;
class Scene;
class LumenGizmo;

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
	EngineCore();
	// Destructor.
	virtual ~EngineCore();
	// Sets the name of the engine (and elements that have names.)
	void setName(const std::string& name);

protected:
	// Set the name of elements in the engine.
	// This should be overridden in the child classes but not called by the user.
	inline virtual void setNameOfElements(const std::string& name) {};
public:

	// ------------------------- //
	//  L U M E N   W I N D O W  //
	// ------------------------- //

	// The window the engine is displayed in.
	LumenWindow* m_parentWindow = nullptr;
	// Display window as a saved document.
	void unsavedDocument();
	// Display window as an unsaved document.
	void savedDocument();
	// Is the mouse hovering the engine?
	bool m_isHovered = false;
	// Is the engine focused?
	bool m_isFocused = false;
	// Get the window the egine is in.
	inline LumenWindow* getLumenWindow() { return m_parentWindow; }
	// Get the content region in the parent window.
	const glm::vec2& getWindowContentRegionSize() const;

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Get the engine's scene.
	Scene& getScene();
	// Main loop where the rendering happens.
	virtual void onRender();
	// Returns the FBO texture ID that can be rendered.
	unsigned int getRenderTexture();
	// Returns the ID of the entity in the pixel coords.
	unsigned int getEntityID(const glm::vec2& pixelCoords);
	// Get the delta time from the app.
	float getDeltaTime() const;

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	virtual void onEvent(const Event& event);
	inline virtual void onMouseButtonEvent(const MouseButtonEvent& event) {}
	inline virtual void onMouseMoveEvent(const MouseMoveEvent& event) {}
	inline virtual void onMouseScrollEvent(const MouseScrollEvent& event) {}
	inline virtual void onMouseDragEvent(const MouseDragEvent& event) {}
	inline virtual void onKeyEvent(const KeyEvent& event) {}
	inline virtual void onWindowResizeEvent(const WindowEvent& event) {}
	inline virtual void onNotifyEvent(const NotifyEvent& event) {}
	inline virtual void onFocusEvent(const NotifyEvent& event) {}
	inline virtual void onDefocusEvent(const NotifyEvent& event) {}
	inline virtual void onHoverEvent(const NotifyEvent& event) {}
	inline virtual void onDehoverEvent(const NotifyEvent& event) {}
	inline virtual void onFileDropEvent(const FileDropEvent& event) {}
	inline virtual void onYamlNodeDropEvent(const YamlNodeDropEvent& event) {}

	// ----------------------- //
	//  C O O R D I N A T E S  //
	// ----------------------- //

	// Get the grid vertex nearest to the provided coordinates.
	glm::vec2 getNearestGridVertex(const glm::vec2& coords);

	// Get the mouse position in the Lumen Window.
	glm::vec2 getMouseLocalPosition();
	// Get the mouse position in the glfw window.
	glm::vec2 getMouseGlobalPosition();

	// Coorinate conversions.
	glm::vec3 pixelToWorldDistance(const glm::vec2& distance);
	glm::vec2 worldToPixelDistance(const glm::vec3& distance);
	glm::vec3 pixelToWorldCoords(const glm::vec2& coords, bool useUpdatedView = false);
	glm::vec2 worldToPixelCoords(const glm::vec3& worldCoords, bool useUpdatedView = false);
	glm::vec2 worldToPixelCoords(const glm::vec2& worldCoords, bool useUpdatedView = false);

	// Convert window coordinates.
	glm::vec2 localToGlobalCoords(const glm::vec2& coords);
	glm::vec2 globalToLocalCoords(const glm::vec2& coords);

	// ----------------------- //
	//  G U I   W I D G E T S  //
	// ----------------------- //

	// Engine design palette.
	// A toolbar at the top of the engine display.
	inline virtual void renderDesignPalette() {};
	inline bool hasDesignPalette()		{ return m_hasDesignPalette;  }
	inline void enableDesignPalette()	{ m_hasDesignPalette = true;  }
	inline void disableDesignPalette()	{ m_hasDesignPalette = false; }

	// Engine overlay.
	// GUI elements that are rendered on top of the engine and block events
	// to the engine.
	inline void enableOverlay()	 { m_hasOverlay = true;  }
	inline void disableOverlay() { m_hasOverlay = false; }
	inline bool hasOverlay()	 { return m_hasOverlay;  }
	inline virtual void renderOverlay() {};

	// Get the gizmo used for manipulation.
	LumenGizmo* getGizmo();

protected:

	// Protected gizmo controls.
	void hideGizmo();
	void visibleGizmo();

private:

	// Friends.
	template <class EngineType>
	friend class GraphicsScene;
	friend class Base2DEngine;
	friend class Base3DEngine;

	// The scene rendered to.
	std::unique_ptr<Scene> m_scene = nullptr;

	// Gizmo!
	std::unique_ptr<LumenGizmo> m_lumenGizmo;

	// Does the engine have a design palette?
	bool m_hasDesignPalette = false;
	bool m_hasOverlay = false;

	// Handlers that are always called on the events.
	// Prevents children from overriding certain behaviour.
	virtual void onNotifyEventForce(const NotifyEvent& event);
	virtual void onMouseMoveEventForce(const MouseMoveEvent& event);
	virtual void onFocusEventForce(const NotifyEvent& event);
	virtual void onDefocusEventForce(const NotifyEvent& event);
	virtual void onHoverEventForce(const NotifyEvent& event);
	virtual void onDehoverEventForce(const NotifyEvent& event);
	virtual void onWindowResizeEventForce(const WindowEvent& event);
	virtual void onFileDropEventForce(const FileDropEvent& event);
	virtual void onYamlNodeDropEventForce(const YamlNodeDropEvent& event);
	virtual void onMouseDragEventForce(const MouseDragEvent& event);
	virtual void onMouseButtonEventForce(const MouseButtonEvent& event);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//