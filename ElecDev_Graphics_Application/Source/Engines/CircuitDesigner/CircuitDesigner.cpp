/*
This is where the interactive 2D design engine is implemented.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"
#include "OpenGL/SceneGL.h"
#include "Graphics/Camera/Camera.h"
#include "OpenGL/Renderer/RendererGL.h"
#include <iostream>
#include "Lumen.h"
#include "Application/Application.h"
#include "OpenGL/Primitives/Grid.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

CircuitDesigner::CircuitDesigner()
	: Base2DEngine()
{
	m_circuit = std::make_shared<Circuit>("Test", "AE");
	getScene().getGrid().disableHelperCircle();
}

CircuitDesigner::~CircuitDesigner()
{
}

void CircuitDesigner::createCircuit(const std::filesystem::path& path) 
{
	m_circuit = std::make_shared<Circuit>(path);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//


