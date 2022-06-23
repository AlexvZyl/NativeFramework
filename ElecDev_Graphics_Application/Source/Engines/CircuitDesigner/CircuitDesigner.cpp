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
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "OpenGL/Primitives/Grid.h"
#include "GUI/LumenGizmo/LumenGizmo.h"
#include "Engines/CircuitDesigner/Peripherals/Cable.h"
#include "Graphics/OpenGL/Primitives/PolyLine.h"
#include "Application/Windows/LumenWindow.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

CircuitDesigner::CircuitDesigner()
	: Base2DEngine()
{
	// Engine setup.
	m_circuit = std::make_shared<Circuit>("Test", "AE");
	enableOverlay();
	enableMenuBar();

	// Setup scene & grid.
	getScene().getGrid()
		.disableHelperCircle()
		.setWidgetPosition(GridWidgetPosition::BOTTOM_RIGHT)
		.setMajorGrid(GridUnit::MILLIMETER, 5);
	getScene().getCamera().scale2D(100.f);

	// Setup gizmo.
	LumenGizmo& gizmo = getGizmo();
	gizmo.setSize(0.1f);
	gizmo.enable();
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
	savePath = path;
}

void CircuitDesigner::renderOverlay() 
{
	//Add indicator for hovered vertices.
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	if (m_hoveredVertexIdx != -1) {
		glm::vec2 pos;
		if (m_activeCable && m_hoveredVertexIdx < m_activeCable->m_polyLine->m_vertices.size()-1) {
			pos = localToGlobalCoords(worldToPixelCoords(m_activeCable->m_polyLine->m_vertices.at(m_hoveredVertexIdx)));
			pos = { pos.x, m_parentWindow->getMainViewportSize().y - pos.y };
			draw_list->AddCircleFilled(pos, clickTol, ImColor(helperColour));
		}
	}
	getScene().getGrid().renderOverlay();
}

void CircuitDesigner::renderMenuBar() 
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


