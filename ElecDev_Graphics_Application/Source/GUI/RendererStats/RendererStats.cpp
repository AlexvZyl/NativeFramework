//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "RendererStats.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "OpenGL/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/Primitives/Vertex.h"

//=======================================================================================================================================//
// Circuit editor.																														 //
//=======================================================================================================================================//

RendererStats::RendererStats(std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags)
{
}

void RendererStats::begin()
{
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
	ImGui::PushItemWidth(-1);
}

void RendererStats::onRender()
{
	// Get current scene.
	Scene* scene = Renderer::getScene();

	if (scene)
	{
		float linesMem = (sizeof(VertexData) * (float)scene->m_linesVAO->m_vertexBufferSize + sizeof(unsigned) * (float)scene->m_linesVAO->m_indexBufferSize) / 1000000;
		float trianglesMem = (sizeof(VertexData) * (float)scene->m_trianglesVAO->m_vertexBufferSize + sizeof(unsigned) * (float)scene->m_trianglesVAO->m_indexBufferSize) / 1000000;
		float texturesMem = (sizeof(VertexDataTextured) * (float)scene->m_texturedTrianglesVAO->m_vertexBufferSize + sizeof(unsigned) * (float)scene->m_texturedTrianglesVAO->m_indexBufferSize) / 1000000;
		float circlesMem = (sizeof(VertexDataCircle) * (float)scene->m_circlesVAO->m_vertexBufferSize + sizeof(unsigned) * (float)scene->m_circlesVAO->m_indexBufferSize) / 1000000;
		float totalMem = linesMem + trianglesMem + texturesMem + circlesMem;
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Scene"))
		{
			ImGui::Text("Total VAO VRAM Usage : %.3f MB", totalMem);
			ImGui::TreePop();
		}
		ImGui::Separator();

		// ---------- //
		// L I N E S  //
		// ---------- //

		ImGui::PushID("LinesVAORendererStats");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Lines VAO"))
		{
			// Setup table
			ImGui::BeginTable("Lines VAO", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
			ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthStretch);
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
			ImGui::TreePop();
		}
		ImGui::PopID();

		// ------------------ //
		// T R I A N G L E S  //
		// ------------------ //

		ImGui::PushID("TrianglesVAORendererStats");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Triangles VAO"))
		{
			// Setup table
			ImGui::BeginTable("Triangles VAO", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
			ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthStretch);
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
			ImGui::TreePop();
		}
		ImGui::PopID();

		// ----------------- //
		//  T E X T U R E S  //
		// ----------------- //

		ImGui::PushID("TexturesVAORendererStats");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Textures VAO"))
		{
			// Setup table
			ImGui::BeginTable("Textures VAO", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
			ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthStretch);
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
			ImGui::TreePop();
		}
		ImGui::PopID();

		// --------------- //
		//  C I R C L E S  //
		// --------------- //

		ImGui::PushID("CirclesVAORendererStats");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Circles VAO"))
		{
			// Setup table
			ImGui::BeginTable("Circles VAO", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
			ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthStretch);
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
			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	else 
	{
		ImGui::Text("No active scene.");
	}
}

void RendererStats::end()
{
	ImGui::PopItemWidth();
	ImGui::End();
}

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//