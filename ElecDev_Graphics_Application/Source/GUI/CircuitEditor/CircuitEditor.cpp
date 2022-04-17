//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "CircuitEditor.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Application/Layers/EngineLayer.h"

//=======================================================================================================================================//
// Circuit editor.																														 //
//=======================================================================================================================================//

CircuitEditor::CircuitEditor(std::string name, int windowFlags)
	: LumenWindow(name, windowFlags)
{
	m_imguiWindowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
}

void CircuitEditor::onImGuiBegin() 
{
	ImGui::Begin(getImGuiName(), &m_isOpen, m_imguiWindowFlags);
}

void CircuitEditor::onImGuiRender() 
{
	Application& app = Lumen::getApp();

	// Get the active engine.
	if (m_trackActiveEngine)
		m_engine = app.getActiveEngine<Design2DEngine>();

	// --------------- //
	//  C R E A T O R  //
	// --------------- //

	if (!m_trackActiveEngine)
	{
		ImGui::Text("Circuit Name");
		ImGui::SameLine();
		ImGui::InputText("##circuitName", &m_circuitNameOnCreation);
		if (ImGui::Button("Create Circuit##CircuitCreatorButton"))
		{
			app.pushEngineLayer<Design2DEngine>(m_circuitNameOnCreation);
			app.queueWindowPop(m_name);
		}
	}

	// ------------- //
	//  E D I T O R  //
	// ------------- //

	else if(m_trackActiveEngine && m_engine)
	{
		ImGui::Text("Circuit Name");
		ImGui::SameLine();
		if (ImGui::InputText("##circuitName", &m_engine->m_circuit->m_label)) 
		{
			auto* layer = Lumen::getApp().getEngineLayer<Design2DEngine>(m_engine);
			layer->setName(m_engine->m_circuit->m_label);
		}
	}

	else 
	{
		ImGui::Text("No active circuit.");
	}
}

void CircuitEditor::onImGuiEnd() 
{
	ImGui::End();
}

void CircuitEditor::setEngine(EngineCore* engine) 
{
	m_engine = dynamic_cast<Design2DEngine*>(engine);
}

void CircuitEditor::setActiveEngineTracking(bool track)
{
	m_trackActiveEngine = track;
}

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//