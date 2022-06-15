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
	CircuitEditor(std::string name, int windowFlags = 0);

	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

private:

	// Name used to create circuits.
	std::string m_circuitNameOnCreation = "Test";

	// Icons.
	inline static unsigned s_cableIcon = NULL;
	inline static unsigned s_componentFileIcon = NULL;

	// Filters used for cables and components.
	ImGuiTextFilter componentsFilter;
	ImGuiTextFilter cablesFilter;
};

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//