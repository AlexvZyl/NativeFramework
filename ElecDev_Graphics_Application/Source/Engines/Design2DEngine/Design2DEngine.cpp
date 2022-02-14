/*
This is where the interactive 2D design engine is implemented.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/CameraGL.h"
#include "OpenGL/RendererGL.h"
#include <iostream>
#include "Lumen.h"
#include "Application/Application.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
Design2DEngine::Design2DEngine()
	: Base2DEngine()
{
	// Default circuit (In this case handled like a default scene).
	m_circuit = std::make_shared<Circuit>("Test", "AE");
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//


