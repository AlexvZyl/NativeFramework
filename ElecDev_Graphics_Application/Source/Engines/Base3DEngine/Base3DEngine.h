#pragma once

/*
The 3D base engine is going to be a child of the 2D engine so that the same pointer can point to the 3D engine.
Some modifications will have to be applied to the matrices and the mouse handling, but other than that the same principles should apply.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// The base 2D engine.
#include "Engines/EngineCore/EngineCore.h"

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

class Camera;
class MouseButtonEvent;
class MouseMoveEvent;
class MouseScrollEvent;
class KeyEvent;

//=============================================================================================================================================//
//  Base 3D Engine Class.																													   //
//=============================================================================================================================================//

// Inherits from the 2D base engine.
class Base3DEngine : public EngineCore
{
public:

	// Constructor
	Base3DEngine();

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Mouse events.
	inline void onMouseButtonEvent(MouseButtonEvent& event) {};
	inline void onMouseMoveEvent(MouseMoveEvent& event) {};
	inline void onMouseScrollEvent(MouseScrollEvent& event) {};

	// Key events.
	inline void onKeyEvent(KeyEvent& event) {};
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//