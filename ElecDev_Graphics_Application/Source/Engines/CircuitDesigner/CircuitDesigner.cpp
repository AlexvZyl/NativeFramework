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
#include "GUI/LumenGizmo/LumenGizmo.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

CircuitDesigner::CircuitDesigner()
	: Base2DEngine()
{
	m_circuit = std::make_shared<Circuit>("Test", "AE");
	getScene().getGrid()
		.disableHelperCircle()
		.setWidgetPosition(GridWidgetPosition::BOTTOM_RIGHT)
		.setMajorGrid(GridUnit::MILLIMETER, 5);
	getScene().getCamera().scale2D(100.f);
	enableOverlay();
	enableDesignPalette();

	LumenGizmo* gizmo = getGizmo();
	gizmo->setSize(0.1f);
	gizmo->enable();
}

CircuitDesigner::~CircuitDesigner()
{
	// Get rid of shared ptr ref.
	m_activeComponent = nullptr;
	m_activeCable = nullptr;
}

void CircuitDesigner::createCircuit(const std::filesystem::path& path) 
{
	m_circuit = std::make_shared<Circuit>(path);
}

void CircuitDesigner::renderOverlay() 
{
	getScene().getGrid().renderOverlay();
}

void CircuitDesigner::renderDesignPalette() 
{
	if (ImGui::Checkbox("Gizmo", &m_gizmoEnabled))
	{
		if (m_gizmoEnabled) visibleGizmo();
		else			    hideGizmo();
	}
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//


