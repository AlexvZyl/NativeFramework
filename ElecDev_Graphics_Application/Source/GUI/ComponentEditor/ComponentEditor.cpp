#pragma once

/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "ComponentEditor.h"
#include "OpenGL/RendererGL.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Port.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Application/Application.h"	
#include "ComponentEditorPopup/ComponentEditorPopup.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"

/*=======================================================================================================================================*/
/* Component Editor.																													 */
/*=======================================================================================================================================*/

ComponentEditor::ComponentEditor(std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags)
{

}

void ComponentEditor::begin() 
{
	// Place editor at correct position.
	/*ImGui::SetNextWindowPos(m_guiState->popUpPosition);*/
	// FIX ME!! The wondow size should be set dynamically
	ImVec4 newCol = ImVec4(0.05f, 0.05f, 0.07f, 0.9f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, newCol);
	ImGui::SetNextWindowSize(ImVec2{ 600.f, 600.f }, ImGuiCond_Once);
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void ComponentEditor::onRender()
{
	// Fetch all the component names
	//auto designEng = Lumen::getApp().m_guiState->design_engine;
	//auto piet = koos->m_circuit->m_components;
	//auto componentsUla = Lumen::getApp().m_guiState->design_engine->m_circuit->m_components;
	//	Fetch The active component.
	Component2D* activeComponent = Lumen::getApp().m_guiState->active_component;
	//check that the active component exists. Close if not.
	if (!activeComponent)
	{
		Lumen::getApp().m_guiState->componentEditor = false;
		return;
	}

	ImGui::Text(" Name:\t");
	ImGui::SameLine();
	if (ImGui::InputText("##ComponentName", &activeComponent->titleString))
	{
		activeComponent->title->updateText(activeComponent->titleString);
	}

	ImGui::Text(" Type:\t");
	ImGui::SameLine();
	ImGui::InputText("##Equipment Type", &activeComponent->equipType);

	// Get Active component type to change component editor based on type

	ImGui::SetNextItemOpen(false, ImGuiCond_Once);
	if (ImGui::TreeNode("Ports"))
	{
		ImGui::BeginTable("Current ports", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit);
		//ImGui::SetColumnWidth(1, 20.f);

			//Setup table
		ImGui::TableSetupColumn("Location    ", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Port Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("I/O Type       ", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		std::vector<std::vector<std::shared_ptr<Port>>> allPorts = { activeComponent->portsWest,
																	 activeComponent->portsEast,
																	 activeComponent->portsNorth,
																	 activeComponent->portsSouth };

		static std::vector<std::string> portPositions = { "Left", "Right", "Top", "Bottom" };

		for (int i = 0; i < allPorts.size(); i++)
		{
			std::vector<std::shared_ptr<Port>> portsSide = allPorts[i];
			int j = 0;
			for (std::shared_ptr<Port> port : portsSide)
			{
				// Table labels.
				char labelName[20];
				sprintf_s(labelName, "##N%d,%d", i, j);
				char labelPos[20];
				sprintf_s(labelPos, "##P%d,%d", i, j);
				char labelType[20];
				sprintf_s(labelType, "##T%d,%d", i, j);
				char labelRemove[20];
				sprintf_s(labelRemove, "Remove##%d,%d", i, j++);

				// Port entry in table.
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				// Position.
				ImGui::PushItemWidth(-1);
				ImGui::Text(portPositions[i].c_str());
				ImGui::PopItemWidth();
				ImGui::TableNextColumn();

				// Name.
				if (ImGui::InputText(labelName, &port->m_label))
					port->title->updateText(port->m_label);

				// Type.
				ImGui::TableNextColumn();
				ImGui::PushItemWidth(-1);
				int* typeval = (int*)&port->m_type;
				ImGui::Combo(labelType, typeval, "IN\0OUT\0IN/OUT");
				ImGui::PopItemWidth();
				ImGui::TableNextColumn();

				// Remove.
				if (ImGui::Button(labelRemove)) 
					activeComponent->removePort(port);

			}
			if (j) ImGui::Separator();
		}

		if (addingPort) 
		{
			//create a table entry for the port to be added
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			//Add the position
			ImGui::PushItemWidth(-1);
			ImGui::Combo("##newPos", &newPos, "Left\0Right\0Top\0Bottom");
			ImGui::PopItemWidth();
			ImGui::TableNextColumn();
			//Add the Name
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##newName", &newName);
			ImGui::PopItemWidth();
			ImGui::TableNextColumn();
			//Add the type
			ImGui::PushItemWidth(-1);
			ImGui::Combo("##newType", &newType, "IN\0OUT\0IN/OUT");
			ImGui::PopItemWidth();
			ImGui::TableNextColumn();
			//Add a "Confirm" button
			if (ImGui::Button("Confirm"))
			{
				// Add the port to the component.
				activeComponent->addPort(newPos, (PortType)newType, newName);
				addingPort = false;
			}
		}
		ImGui::EndTable();
		if (!addingPort)
		{
			if (ImGui::Button("New Port"))
			{
				addingPort = true;
			}
		}
		ImGui::TreePop();
	}

	// --------------------- //
	//  D A T A   T A B L E  //
	// --------------------- //

	const char* buffer[100];

	int i = 0;

	for (auto& [key, val] : activeComponent->cableDict)
	{

		buffer[i] = key.c_str();
		i++;
	}

	ImGui::SetNextItemOpen(false, ImGuiCond_Once);
	if (ImGui::TreeNode("Data Automation"))
	{
		// Add dict entry.
		static std::string entryToAdd;
		ImGui::Text("Add an attribute to the dictionary:");
		ImGui::InputText("##DictEntry", &entryToAdd);
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			activeComponent->cableDict.insert({entryToAdd, "From(Circuit Database)"});
			entryToAdd = "";
		}

		// Setup table.
		ImGui::BeginTable("Columns to specify", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollX 
						| ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp
						| ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY, ImVec2(600.0, 400.0));
		
		// Setup header.
		ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Function", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		// Store entries to be removed.
		static std::vector<std::string> toRemove;
		toRemove.reserve(1);

		
		// Table.
		for (auto& [key, val]: activeComponent->cableDict) 
		{
			// ID.
			ImGui::PushID((int)key.c_str());

			// Selectable.
			bool isOpen = true;
			ImGui::TableNextRow();
			
			// Dict data.
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(key.c_str());
			ImGui::TableSetColumnIndex(1);
			ImGui::InputText("##Input", &val);
			ImGui::TableSetColumnIndex(2);
			// Remove button.
			if (ImGui::Button("Remove"))
				toRemove.push_back(key);
			// ID.
			ImGui::PopID();
		}

		// Cleanup table.
		
		ImGui::EndTable();
		
		ImGui::TreePop();

		// Remove entries.
		for (auto& key : toRemove)
			activeComponent->cableDict.erase(key);
		toRemove.clear();
	}

	// --------------------- //
	//     FROM SELECTION    //
	// --------------------- //

	const char* fromSelection[] = {"Circuit Database", "Motor Database", "Cable Database"};

	std::string from = "FROM(";

	std::string end = ")";

	ImGui::SetNextItemOpen(false, ImGuiCond_Once);
	if (ImGui::TreeNode("From"))
	{
		// int* typeval2 = (int*)&activeComponent->cableDict;
		ImGui::Combo("Select Column##From", &fromSelector, buffer, activeComponent->cableDict.size());

		ImGui::Combo("Select Database##From2", &databaseSelector, fromSelection, IM_ARRAYSIZE(fromSelection));
		// ImGui::Text("Hello World");

		if (ImGui::Button("Insert From function"))
		{
			from += fromSelection[databaseSelector] + end;
			activeComponent->cableDict[buffer[fromSelector]] = from;
		}

		ImGui::TreePop();
	}

	// ------------ //
	//     SIZE     //
	// ------------ //

	ImGui::SetNextItemOpen(false, ImGuiCond_Once);
	if (ImGui::TreeNode("Size"))
	{
		ImGui::Combo("Select Column##size", &sizeSelector, buffer, activeComponent->cableDict.size());
		// ImGui::Text(std::to_string(typeval3).c_str());
		if (ImGui::Button("Insert Size function"))
		{
			activeComponent->cableDict[buffer[sizeSelector]] = "Size()";
		}
		ImGui::TreePop();
	}

	// --------------------- //
	//      IF STATEMENT     //
	// --------------------- //

	// This should be the number of components of a specific type or the names of the components
	const char* ifRowSelection[] = { "0", "1", "2", "3" };

	std::string ifString = "IF(";

	std::string forwardBracket = "[";

	std::string backwardBracket = "]";

	std::string comma = ",";

	ImGui::SetNextItemOpen(false, ImGuiCond_Once);
	if (ImGui::TreeNode("IF"))
	{
		ImGui::Combo("Select Column##IF", &ifSelector, buffer, activeComponent->cableDict.size());
		ImGui::Combo("Select Variable To Compare##IF", &ifSelector2, buffer, activeComponent->cableDict.size());
		ImGui::Combo("Select Equipment##IF2", &equipmentSelector, ifRowSelection, IM_ARRAYSIZE(ifRowSelection));
		ImGui::Combo("Select Comparator##IF3", &comparatorSelector, comparatorSelection, IM_ARRAYSIZE(comparatorSelection));
		ImGui::InputText("##Comparison Value", &comparisonValue);
		ImGui::InputText("##True Statement", &trueStatement);
		ImGui::InputText("##False Statement", &falseStatement);
		if (ImGui::Button("Insert IF function"))
		{
			if (trueStatement.find(comma) != std::string::npos) {
				trueStatement = forwardBracket + trueStatement + backwardBracket;
			}
			if (comparisonValue.find(comma) != std::string::npos) {
				comparisonValue = forwardBracket + comparisonValue + backwardBracket;
			}

			ifString += buffer[ifSelector2] + comma + comparatorSelection[comparatorSelector] + comma + comparisonValue + comma + trueStatement + comma + falseStatement + end;
			activeComponent->cableDict[buffer[ifSelector]] = ifString;
		}

		ImGui::TreePop();
	}

	// --------------------- //
	//      COMBINE TEXT     //
	// --------------------- //

// This should be the number of components of a specific type or the names of the components
	std::string combineText = "combine_text(";

	std::string plusString = "+";

	ImGui::SetNextItemOpen(false, ImGuiCond_Once);
	if (ImGui::TreeNode("Combine Text"))
	{
		ImGui::Combo("Select Column##Combine", &combineSelector, buffer, activeComponent->cableDict.size());
		if (ImGui::Combo("Select Variable##Combine", &combineSelectorVariable, buffer, activeComponent->cableDict.size())) 
    {
			combineTextString += buffer[combineSelectorVariable] + plusString;
		}
		ImGui::InputText("##Combine String", &combineTextString);
		if (ImGui::Button("Insert Combine function"))
		{
			combineTextString = combineTextString.substr(0, combineTextString.size() - 1);
			combineText += combineTextString + end;
			activeComponent->cableDict[buffer[combineSelector]] = combineText;
		}
		ImGui::TreePop();
	}

}

void ComponentEditor::end()
{
	ImGui::PopStyleColor();
	ImGui::End();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/