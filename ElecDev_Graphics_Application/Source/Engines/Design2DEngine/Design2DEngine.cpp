/*
This is where the interactive 2D design engine is implemented.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
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

// Constructor.
Design2DEngine::Design2DEngine()
	: Base2DEngine()
{
	m_circuit = std::make_shared<Circuit>("Test", "AE");
	getScene().getGrid().disableHelperCircle();
}

Design2DEngine::~Design2DEngine()
{
	m_activeComponent = nullptr;
	m_activeCable = nullptr;
	m_circuit.reset();
}

void Design2DEngine::createCircuit(const std::filesystem::path& path) 
{
	m_circuit.reset();
	m_circuit = std::make_shared<Circuit>(path);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//


