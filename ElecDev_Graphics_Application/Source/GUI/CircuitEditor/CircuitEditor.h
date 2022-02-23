#pragma once

//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"

//=======================================================================================================================================//
// Forward declerations.																												 //
//=======================================================================================================================================//

class EngineCore;

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

private:

	EngineCore* m_engine = nullptr;

};

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//