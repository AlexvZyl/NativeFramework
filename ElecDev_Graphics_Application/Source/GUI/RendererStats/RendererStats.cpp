//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "RendererStats.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "Utilities/Profiler/Profiler.h"

//=======================================================================================================================================//
// Circuit editor.																														 //
//=======================================================================================================================================//

RendererStats::RendererStats(std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags)
{}

void RendererStats::begin()
{
	ImGui::Begin(m_name.c_str(), NULL, m_imguiWindowFlags);
	ImGui::PushItemWidth(-1);
}

void RendererStats::onRender()
{
	Scene* scene = Renderer::getScene();
	Application& app = Lumen::getApp();

	// Update frame count used for fps calculations.
	// Allows a consistent update time independant of FPS.
	if (app.m_waitForEvents)
	{
		m_framesPerAverage = (1.f / app.m_eventsTimeout) * m_updateTime;
	}
	else 
	{
		m_framesPerAverage = app.m_targetFPS * m_updateTime;
	}

	// ----------------- //
	//  P R O F I L E R  //
	// ----------------- //

	ImGui::PushID("ProfilerResults");
	if (ImGui::BeginChild("Child", { 0, m_contentRegionSize.y / 5.f }, true, ImGuiWindowFlags_AlwaysAutoResize))
	{
		// Ensure profilder is enabled.
		app.m_profilerActive = true;

		// Iterate over profiler restults and use to calculate average.
		for (auto& result : app.m_profilerResults)
		{
			// Ensure entry.
			m_currentFrameAverages.insert({ result.name, 0 });
			// Update average.
			m_currentFrameAverages.at(result.name) += result.msTime / m_framesPerAverage;
		}
		// Done with frame.
		m_currentFrame++;
		if (m_currentFrame > m_framesPerAverage - 1)
		{
			m_prevFrameAverages = m_currentFrameAverages;
			m_currentFrameAverages.clear();
			m_currentFrame = 0;
		}

		// Startup message.
		if (!m_prevFrameAverages.size())
		{
			ImGui::Text("Gathering initial data...");
		}
		// Performance table.
		else
		{
			// Setup table
			ImGui::BeginTable("Profiler", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp);
			ImGui::TableSetupColumn("Pipeline", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Time (ms/frame)", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();

			// Log results in table.
			float renderLayersTime = 0;
			float renderScenesTime = 0;
			float imGuiTime = 0;
			float fps = 0;
		
			// Display the calculated averages.
			for (auto& [name, time] : m_prevFrameAverages)
			{
				// Keep track of these values to track imgui calls time.
				// Drawing scenes.
				if (name == "Draw Scene")
				{
					renderScenesTime += time;
					continue;
				}
				else if (name == "Frametime")
				{
					fps = 1.f / (time * 1e-3);
				}

#ifdef PROFILE_IMGUI_OVERHEAD
				// Rendering all of the layers.
				else if (name == "App OnRender")
				{
					renderLayersTime = time;
					continue;
				}

				// ImGui functions.
				else if (result.name == "ImGui NewFrame" ||
						 result.name == "ImGui OnUpdate" ||
						 result.name == "ImGui Draw")
				{
					imGuiTime += time;
					continue;
				}
#endif

				// Log the scope time.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(name.c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%.3f", time);
			}

#ifdef PROFILE_IMGUI_OVERHEAD
			// ImGui overhead (Drawing, calling functions, new frame).
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("ImGui Overhead");
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%.3f", renderLayersTime - renderScenesTime + imGuiTime);
#endif

			// Rendering scenes.
			if (renderScenesTime)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Draw Scenes");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%.3f", renderScenesTime);
			}

			// Done.
			ImGui::EndTable();
			// Clear profiler results.
			app.m_profilerResults.reserve(app.m_profilerResults.size());
			app.m_profilerResults.shrink_to_fit();
			app.m_profilerResults.clear();

			// Display fps.
			ImGui::Text("Estimated FPS: ");
			ImGui::SameLine();
			 static glm::vec4 fpsCol = {0.4f, 0.4f, 1.f, 1.f};
			ImGui::TextColored(fpsCol, std::to_string((int)fps).c_str());
			ImGui::TextWrapped("Disable 'Wait Events' for more accurate results.");
			ImGui::Checkbox("  Wait Events", &app.m_waitForEvents);
		}		
	}
	else
	{
		// Disable profiler.
		app.m_profilerActive = false;
		// Clear profiler results.
		app.m_profilerResults.reserve(app.m_profilerResults.size());
		app.m_profilerResults.shrink_to_fit();
		app.m_profilerResults.clear();
	}
	
	ImGui::EndChild();
	ImGui::PopID();

	// ----------------- //
	//  R E N D E R E R  //
	// ----------------- //

	ImGui::PushID("RendererData");
	if (ImGui::BeginChild("Child", { 0, m_contentRegionSize.y / 5.f }, true))
	{
		// Setup table
		ImGui::BeginTable("RendererTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp);
		ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		// Draw calls.
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Draw Calls");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%d", app.m_rendererData.drawCalls);

		// Draw calls.
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Render Passes");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%d", app.m_rendererData.renderPasses);

		// Done.
		ImGui::EndTable();
		app.m_rendererData.reset();

		// Pipeline.
		for (auto& [key, value] : Renderer::s_pipelineControls)
		{
			ImGui::Checkbox(key.c_str(), &value);
		}
	}
	else
	{
		app.m_rendererData.reset();
	}
	
	ImGui::EndChild();
	ImGui::PopID();

	// ----------- //
	//  S C E N E  //
	// ----------- //

	ImGui::PushID("Scene");
	if (ImGui::BeginChild("Child", { 0, 0 }, true))
	{
		if (scene)
		{
			// Calculate memory usage.
			float linesMem = (sizeof(VertexData) * (float)scene->m_linesVAO->m_vertexBufferSize + sizeof(unsigned) * (float)scene->m_linesVAO->m_indexBufferSize) / 1000000;
			float trianglesMem = (sizeof(VertexData) * (float)scene->m_trianglesVAO->m_vertexBufferSize + sizeof(unsigned) * (float)scene->m_trianglesVAO->m_indexBufferSize) / 1000000;
			float texturesMem = (sizeof(VertexDataTextured) * (float)scene->m_texturedTrianglesVAO->m_vertexBufferSize + sizeof(unsigned) * (float)scene->m_texturedTrianglesVAO->m_indexBufferSize) / 1000000;
			float circlesMem = (sizeof(VertexDataCircle) * (float)scene->m_circlesVAO->m_vertexBufferSize + sizeof(unsigned) * (float)scene->m_circlesVAO->m_indexBufferSize) / 1000000;
			float totalMem = linesMem + trianglesMem + texturesMem + circlesMem;
			ImGui::Text("Total VAO VRAM Usage : %.3f MB", totalMem);

			// ---------- //
			// L I N E S  //
			// ---------- //

			ImGui::PushID("LinesVAORendererStats");
			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("Lines VAO"))
			{
				// Setup table
				ImGui::BeginTable("Lines VAO", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
				ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();
				// Vertex Count.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Vertex Count");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_linesVAO->m_vertexCount);

				// Index Count.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Index Count");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_linesVAO->m_indexCount);

				// Vertex Buffer Capacity.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Vertex Buffer Capacity");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_linesVAO->m_vertexBufferSize);

				// Index Buffer Capacity.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Index Buffer Capacity");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_linesVAO->m_indexBufferSize);

				// VAO Memory usage.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("VRAM Usage");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%.3f MB", linesMem);

				// Done.
				ImGui::EndTable();
			}
			ImGui::PopID();

			// ------------------ //
			// T R I A N G L E S  //
			// ------------------ //

			ImGui::PushID("TrianglesVAORendererStats");
			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("Triangles VAO"))
			{
				// Setup table
				ImGui::BeginTable("Triangles VAO", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
				ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				// Vertex Count.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Vertex Count");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_trianglesVAO->m_vertexCount);

				// Index Count.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Index Count");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_trianglesVAO->m_indexCount);

				// Vertex Buffer Capacity.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Vertex Buffer Capacity");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_trianglesVAO->m_vertexBufferSize);

				// Index Buffer Capacity.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Index Buffer Capacity");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_trianglesVAO->m_indexBufferSize);

				// VAO Memory usage.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("VRAM Usage");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%.3f MB", trianglesMem);

				// Done.
				ImGui::EndTable();
			}
			ImGui::PopID();

			// ----------------- //
			//  T E X T U R E S  //
			// ----------------- //

			ImGui::PushID("TexturesVAORendererStats");
			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("Textures VAO"))
			{
				// Setup table
				ImGui::BeginTable("Textures VAO", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
				ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				// Vertex Count.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Vertex Count");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_texturedTrianglesVAO->m_vertexCount);

				// Index Count.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Index Count");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_texturedTrianglesVAO->m_indexCount);

				// Vertex Buffer Capacity.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Vertex Buffer Capacity");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_texturedTrianglesVAO->m_vertexBufferSize);

				// Index Buffer Capacity.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Index Buffer Capacity");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_texturedTrianglesVAO->m_indexBufferSize);

				// VAO Memory usage.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("VRAM Usage");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%.3f MB", texturesMem);

				// Done.
				ImGui::EndTable();
			}
			ImGui::PopID();

			// --------------- //
			//  C I R C L E S  //
			// --------------- //

			ImGui::PushID("CirclesVAORendererStats");
			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("Circles VAO"))
			{
				// Setup table
				ImGui::BeginTable("Circles VAO", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
				ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				// Vertex Count.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Vertex Count");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_circlesVAO->m_vertexCount);

				// Index Count.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Index Count");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_circlesVAO->m_indexCount);

				// Vertex Buffer Capacity.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Vertex Buffer Capacity");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_circlesVAO->m_vertexBufferSize);

				// Index Buffer Capacity.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Index Buffer Capacity");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", scene->m_circlesVAO->m_indexBufferSize);

				// VAO Memory usage.
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("VRAM Usage");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%.3f MB", circlesMem);

				// Done.
				ImGui::EndTable();
			}
			ImGui::PopID();
		}
		else
		{
			ImGui::Text("No active scene.");
		}
		
	}
	ImGui::EndChild();
	ImGui::PopID();
}

void RendererStats::end()
{
	ImGui::PopItemWidth();
	ImGui::End();

	// Make sure the data is cleared, since it is not being 
	// cleared in this case.
	Application& app = Lumen::getApp();
	if (!app.m_profilerActive)
	{
		app.m_rendererData.reset();
		app.m_profilerResults.reserve(app.m_profilerResults.size());
		app.m_profilerResults.shrink_to_fit();
		app.m_profilerResults.clear();
	}
}

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//