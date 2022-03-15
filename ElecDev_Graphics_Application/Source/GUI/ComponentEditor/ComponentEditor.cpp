#pragma once

/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "ComponentEditor.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Application/Application.h"	
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Engines/Design2DEngine/Peripherals/Cable.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Port.h"

/*=======================================================================================================================================*/
/* Component Editor.																													 */
/*=======================================================================================================================================*/

ComponentEditor::ComponentEditor(std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags)
{}

void ComponentEditor::begin()
{
	// Place editor at correct position.
	/*ImGui::SetNextWindowPos(m_guiState->popUpPosition);*/
	// FIX ME!! The wondow size should be set dynamically
	ImGui::SetNextWindowSize(ImVec2{ 600.f, 600.f }, ImGuiCond_Once);
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void ComponentEditor::onRender()
{
	Application& app = Lumen::getApp();

	if (!app.m_guiState->design_engine) return;

	// Fetch all the component names.
	auto& numComponents = app.m_guiState->design_engine->m_circuit->m_components;
	auto& numCables = app.m_guiState->design_engine->m_circuit->m_cables;
	const char* componentNames[100];
	int numCom = 0;
	for (auto& key : numComponents)
	{
		componentNames[numCom] = key->titleString.c_str();
		numCom++;
	}

	// Fetch active elements.
	Component2D* activeComponent = app.m_guiState->design_engine->m_activeComponent.get();
	Cable* activeCable = app.m_guiState->design_engine->m_activeCable.get();

	// Check that the active component exists. Close if not.
	if (activeComponent)
	{
		ImGui::Text(" Name:\t");
		ImGui::SameLine();
		if (ImGui::InputText("##ComponentName", &activeComponent->titleString))
			activeComponent->title->updateText(activeComponent->titleString);

		ImGui::Text(" Type:\t  ");
		ImGui::SameLine();
		ImGui::InputText("##Equipment Type", &activeComponent->equipType);

		if (ImGui::BeginChild("PortsChild", {0, m_contentRegionSize.y / 4.5f}, true))
		{
			// Setup table.
			ImGui::BeginTable("Current ports", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp);
			ImGui::TableSetupColumn("Location    ", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Port Name", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("I/O Type       ", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed);
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
					ImGui::PushItemWidth(-1);
					if (ImGui::InputText(labelName, &port->m_label))
						port->title->updateText(port->m_label);
					ImGui::PopItemWidth();
					ImGui::TableNextColumn();
					// Type.
					ImGui::PushItemWidth(-1);
					int* typeval = (int*)&port->m_type;
					ImGui::Combo(labelType, typeval, "IN\0OUT\0IN/OUT");
					ImGui::PopItemWidth();
					ImGui::TableNextColumn();
					// Remove.
					ImGui::PushItemWidth(-1);
					if (ImGui::Button(labelRemove))
						activeComponent->removePort(port);
					ImGui::PopItemWidth();
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
				ImGui::PushItemWidth(-1);
				if (ImGui::Button("Confirm"))
				{
					// Add the port to the component.
					activeComponent->addPort(newPos, (PortType)newType, newName);
					addingPort = false;
				}
				ImGui::PopItemWidth();
			}
			ImGui::EndTable();

			if (!addingPort)
			{
				if (ImGui::Button("New Port"))
					addingPort = true;
			}			
		}
		ImGui::EndChild();
	}
	// Cable properties.
	if (activeCable)
	{
		ImGui::Text(" Name:\t");
		ImGui::SameLine();
		if (ImGui::InputText("##ComponentName", &activeCable->m_titleString))
		{
			activeCable->m_title1->updateText(activeCable->m_titleString);
			activeCable->m_title2->updateText(activeCable->m_titleString);
		}
		ImGui::Text(" Type:\t Cable");
	}

	// --------------------- //
	//  D A T A   T A B L E  //
	// --------------------- //

	ImGui::PushID("CompEdChildData");
	if (ImGui::BeginChild("DataChild", { 0,0 }, true))
	{

		if (activeComponent || activeCable)
		{
			std::unordered_map<std::string, std::string> dataDict;
			if (activeComponent) dataDict = activeComponent->dataDict;
			else if (activeCable) dataDict = activeCable->cableDict;

			const char* buffer[100];
			int numKeys = 0;

			if (activeComponent)
			{
				for (auto& [key, val] : activeComponent->dataDict)
				{
					buffer[numKeys] = key.c_str();
					numKeys++;
				}
			}
			else if (activeCable)
			{
				for (auto& [key, val] : activeCable->cableDict)
				{
					buffer[numKeys] = key.c_str();
					numKeys++;
				}
			}

			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("Data Automation"))
			{
				// Add dict entry.
				static std::string entryToAdd;
				ImGui::Text("Add an attribute to the dictionary:");
				ImGui::InputText("##DictEntry", &entryToAdd);
				ImGui::SameLine();
				if (ImGui::Button("Add"))
				{
					if (activeComponent)
					{
						activeComponent->dataDict.insert({ entryToAdd, "From(Circuit Database)" });
					}
					else
					{
						activeCable->cableDict.insert({ entryToAdd, "From(Circuit Database)" });
					}
					entryToAdd = "";
				}

				// Dimension of Table
				int height;
				if (numKeys < 10) height = 50 + 27 * (numKeys - 1);
				else height = 300;

				// Setup table.
				ImGui::BeginTable("Columns to specify", 3, ImGuiTableFlags_Resizable
					| ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp
					| ImGuiTableFlags_Borders, ImVec2(0, height));

				// Setup header.
				ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Function", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableHeadersRow();

				// Store entries to be removed.
				static std::vector<std::string> toRemove;
				toRemove.reserve(1);

				// Table.
				int keyCount = 0;
				for (auto& [key, val] : dataDict)
				{
					// ID.
					ImGui::PushID(keyCount++);

					// Selectable.
					bool isOpen = true;
					ImGui::TableNextRow();

					// Dict data.
					ImGui::TableSetColumnIndex(0);
					ImGui::PushItemWidth(-1);
					ImGui::Text(key.c_str());
					ImGui::PopItemWidth();
					ImGui::TableSetColumnIndex(1);
					ImGui::PushItemWidth(-1);
					ImGui::InputText("##Input", &val);
					ImGui::PopItemWidth();
					ImGui::TableSetColumnIndex(2);
					// Remove button.
					ImGui::PushItemWidth(-1);
					if (ImGui::Button("Remove"))
					{
						toRemove.push_back(key);
					}
					ImGui::PopItemWidth();
					// ID.
					ImGui::PopID();
				}

				// Cleanup table.
				ImGui::EndTable();

				// Remove entries.
				for (auto& key : toRemove)
				{
					if (activeComponent)
					{
						activeComponent->dataDict.erase(key);
					}
					else
					{
						activeCable->cableDict.erase(key);
					}
				}
				toRemove.clear();
			}

			// --------------------- //
			//     FROM SELECTION    //
			// --------------------- //

			const char* fromSelection[] = { "Circuit Database", "Motor Database", "Cable Database" };
			std::string from = "FROM(";
			std::string end = ")";

			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("From"))
			{
				// int* typeval2 = (int*)&dataDict;
				ImGui::Combo("Select Column##From", &fromSelector, buffer, dataDict.size());

				ImGui::Combo("Select Database##From2", &databaseSelector, fromSelection, IM_ARRAYSIZE(fromSelection));
				// ImGui::Text("Hello World");

				if (ImGui::Button("Insert From function"))
				{
					from += fromSelection[databaseSelector] + end;
					if (activeComponent)
					{
						activeComponent->dataDict[buffer[fromSelector]] = from;
					}
					else
					{
						activeCable->cableDict[buffer[fromSelector]] = from;
					}
				}
			}

			// ------------ //
			//     SIZE     //
			// ------------ //

			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("Size"))
			{
				ImGui::Combo("Select Column##size", &sizeSelector, buffer, dataDict.size());
				// ImGui::Text(std::to_string(typeval3).c_str());
				if (ImGui::Button("Insert Size function"))
				{
					if (activeComponent)
					{
						activeComponent->dataDict[buffer[fromSelector]] = "Size()";
					}
					else
					{
						activeCable->cableDict[buffer[fromSelector]] = "Size()";
					}
				}
			}

			// --------------------- //
			//      IF STATEMENT     //
			// --------------------- //

			// This should be the number of components of a specific type or the names of the components
			std::string ifString = "IF(";
			std::string forwardBracket = "[";
			std::string backwardBracket = "]";
			std::string comma = ",";

			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("IF"))
			{
				ImGui::Combo("Select Column##IF", &ifSelector, buffer, dataDict.size());
				ImGui::Combo("Select Variable To Compare##IF", &ifSelector2, buffer, dataDict.size());
				ImGui::Combo("Select Equipment##IF2", &equipmentSelector, componentNames, numCom);
				ImGui::Combo("Select Comparator##IF3", &comparatorSelector, comparatorSelection, IM_ARRAYSIZE(comparatorSelection));
				ImGui::InputText("##Comparison Value", &comparisonValue);
				ImGui::InputText("##True Statement", &trueStatement);
				ImGui::InputText("##False Statement", &falseStatement);
				if (ImGui::Button("Insert IF function"))
				{
					if (trueStatement.find(comma) != std::string::npos)
					{
						trueStatement = forwardBracket + trueStatement + backwardBracket;
					}
					if (comparisonValue.find(comma) != std::string::npos)
					{
						comparisonValue = forwardBracket + comparisonValue + backwardBracket;
					}
					ifString += buffer[ifSelector2] + comma + comparatorSelection[comparatorSelector] + comma + comparisonValue + comma + trueStatement + comma + falseStatement + end;
					dataDict[buffer[ifSelector]] = ifString;
				}
			}

			// --------------------- //
			//      COMBINE TEXT     //
			// --------------------- //

			// This should be the number of components of a specific type or the names of the components.
			std::string combineText = "combine_text(";
			std::string plusString = "+";
			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("Combine Text"))
			{
				ImGui::Combo("Select Column##Combine", &combineSelector, buffer, dataDict.size());
				if (ImGui::Combo("Select Variable##Combine", &combineSelectorVariable, buffer, dataDict.size()))
				{
					combineTextString += buffer[combineSelectorVariable] + plusString;
				}
				ImGui::InputText("##Combine String", &combineTextString);
				if (ImGui::Button("Insert Combine function"))
				{
					combineTextString = combineTextString.substr(0, combineTextString.size() - 1);
					combineText += combineTextString + end;
					if (activeComponent)
					{
						activeComponent->dataDict[buffer[combineSelector]] = combineText;
					}
					else {
						activeCable->cableDict[buffer[combineSelector]] = combineText;
					}
				}
			}
		}
	}
	ImGui::PopID(); 
	ImGui::EndChild();
}

void ComponentEditor::end()
{
	ImGui::End();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/