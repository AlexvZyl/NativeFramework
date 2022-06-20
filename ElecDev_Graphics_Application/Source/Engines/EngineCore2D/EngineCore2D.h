#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/EngineCore/EngineCore.h"

//=============================================================================================================================================//
//  Base 2D Engine Class.																													   //
//=============================================================================================================================================//

class EngineCore2D : public EngineCore
{

public:

	// Contructor.
	EngineCore2D();
	// Destructor.
	inline virtual ~EngineCore2D() override = default;

	// Mouse event basic controls.
	// Implements the basic camera controls that can be used by the children.
	void onMouseDragEvent(const MouseDragEvent& event) override;
	void onMouseScrollEvent(const MouseScrollEvent& event) override;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//