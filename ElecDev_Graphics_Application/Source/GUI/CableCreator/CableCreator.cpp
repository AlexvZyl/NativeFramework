//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "CableCreator.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Cable.h"
#include "Resources/ResourceHandler.h"

//=======================================================================================================================================//
// Circuit editor.																														 //
//=======================================================================================================================================//

CableCreator::CableCreator(std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags)
{}

void CableCreator::begin()
{
	ImGui::SetNextWindowSize({500, 500}, ImGuiCond_Once);
	glm::vec2 vpSize = ImGui::GetMainViewport()->Size;
	ImGui::SetNextWindowPos({ vpSize.x / 2, vpSize.y / 2 }, ImGuiCond_Once, {0.5f, 0.5f});
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void CableCreator::onRender()
{

}

void CableCreator::end()
{
	ImGui::End();
}

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//