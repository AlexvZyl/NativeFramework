#pragma once

//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"

//=======================================================================================================================================//
// Forward declerations.																												 //
//=======================================================================================================================================//

class EngineCore;
class Design2DEngine;

//=======================================================================================================================================//
// Component Editor.																													 //
//=======================================================================================================================================//

class CircuitEditor : public GuiElementCore
{
public:

	// Constructor.
	CircuitEditor(std::string name, int windowFlags);

	// Rendering functions.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

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