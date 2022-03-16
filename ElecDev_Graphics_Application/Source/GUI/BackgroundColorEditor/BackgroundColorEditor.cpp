//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "BackgroundColorEditor.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Cable.h"
#include "OpenGL/SceneGL.h"

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

BackgroundColorEditor::BackgroundColorEditor(std::string name, int imguiWindowFlags)
	: GuiElementCore(name, imguiWindowFlags)
{}

void BackgroundColorEditor::begin()
{
	ImGui::SetNextWindowSize(glm::vec2(400, 350), ImGuiCond_Once);
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void BackgroundColorEditor::onRender()
{
	Application& app = Lumen::getApp();
	Scene* scene = nullptr;

	if (app.m_guiState->design_engine)
	{
		scene = app.m_guiState->design_engine->m_scene.get();
	}

	// Open color editor.
	if (scene)
	{
		glm::vec4 color;
		color[0] = scene->m_backgroundVAO->m_vertexCPU[0].data.color[0];
		color[1] = scene->m_backgroundVAO->m_vertexCPU[0].data.color[1];
		color[2] = scene->m_backgroundVAO->m_vertexCPU[0].data.color[2];
		color[3] = scene->m_backgroundVAO->m_vertexCPU[0].data.color[3];
		ImGui::SameLine();
		if (ImGui::ColorPicker4("##ColorEditor", &color[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf))
		{
			for (auto& vertex : scene->m_backgroundVAO->m_vertexCPU)
			{
				vertex.data.color[0] = color[0];
				vertex.data.color[1] = color[1]; 
				vertex.data.color[2] = color[2];
				vertex.data.color[3] = color[3];

			}
			scene->m_backgroundVAO->m_vertexBufferSynced = false;
		}
	}
	else 
	{
		ImGui::Text("No active scene.");
	}
}

void BackgroundColorEditor::end()
{
	ImGui::End();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
