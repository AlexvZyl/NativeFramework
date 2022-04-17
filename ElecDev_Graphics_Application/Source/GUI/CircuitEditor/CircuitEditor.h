#pragma once

//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "Application/LumenWindow/LumenWindow.h"

//=======================================================================================================================================//
// Forward declerations.																												 //
//=======================================================================================================================================//

class EngineCore;
class Design2DEngine;

//=======================================================================================================================================//
// Component Editor.																													 //
//=======================================================================================================================================//

class CircuitEditor : public LumenWindow
{
public:

	// Constructor.
	CircuitEditor(std::string name, int windowFlags);

	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

	// Set the engine that the editor belongs to.
	void setEngine(EngineCore* engine);

	// Set if the editor should track the active engine.
	void setActiveEngineTracking(bool track);

private:

	// Name used to create circuits.
	std::string m_circuitNameOnCreation = "";

	// The engine the editor belongs to.
	Design2DEngine* m_engine = nullptr;

	// Should the editor track the active engine?
	bool m_trackActiveEngine = false;

};

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//