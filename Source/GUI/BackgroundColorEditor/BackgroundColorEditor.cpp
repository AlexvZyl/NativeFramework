//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "BackgroundColorEditor.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Component2D.h"
#include "Engines/CircuitDesigner/Peripherals/Cable.h"
#include "OpenGL/SceneGL.h"

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

BackgroundColorEditor::BackgroundColorEditor(std::string name, int imguiWindowFlags)
	: LumenWindow(name.c_str(), imguiWindowFlags)
{}

void BackgroundColorEditor::onImGuiBegin()
{
	ImGui::SetNextWindowSize(glm::vec2(400, 350), ImGuiCond_Once);
	ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
}

void BackgroundColorEditor::onImGuiRender()
{
	Application& app = Lumen::getApp();

	// Get the active scene from the engine.
	EngineCore* engine = app.getActiveEngine();
	Scene* scene = nullptr;
	if (engine)
	{
		scene = &engine->getScene();
	}

	// Open color editor.
	if (scene)
	{
		glm::vec4 color = scene->m_backgroundBuffer->getVertex(0).color;
		ImGui::SameLine();
		if (ImGui::ColorPicker4("##ColorEditor", &color[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf))
		{
			scene->m_backgroundBuffer->getVertexData().iterateElements();
			for (auto& vertex : scene->m_backgroundBuffer->getVertexData())
			{
				vertex.color[0] = color[0];
				vertex.color[1] = color[1]; 
				vertex.color[2] = color[2];
				vertex.color[3] = color[3];

			}
			scene->m_backgroundBuffer->getVertexData().iterateMemory();
			scene->m_backgroundBuffer->reloadVertices();
		}
	}

	// No scene to change the color of.
	else ImGui::Text("No active scene.");
}

void BackgroundColorEditor::onImGuiEnd()
{
	ImGui::End();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
