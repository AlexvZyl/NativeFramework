#pragma once

/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "ComponentEditor.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Application/Application.h"	
#include "Application/LumenWindow/LumenWindow.h"
#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/ComponentDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"
#include "Engines/CircuitDesigner/Peripherals/Cable.h"
#include "Engines/CircuitDesigner/Peripherals/Component2D.h"
#include "Engines/CircuitDesigner/Peripherals/Port.h"
#include "Resources/ResourceHandler.h"
#include "Lumen.h"
#include "GUI/LumenPayload/LumenPayload.h"

/*=======================================================================================================================================*/
/* Component Editor.																													 */
/*=======================================================================================================================================*/

ComponentEditor::ComponentEditor(std::string name, int windowFlags)
	: LumenWindow(name, windowFlags)
{
	if (!s_iconCreated)
	{
		s_cableIcon = loadBitmapToGL(loadImageFromResource(CABLE_ICON));
		s_textHeight = ImGui::CalcTextSize("A").y;
		s_iconCreated = true;
	}
}

void ComponentEditor::onImGuiBegin()
{
	ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
}

void ComponentEditor::onImGuiRender()
{
	Application& app = Lumen::getApp();

	CircuitDesigner* design_engine = app.getActiveEngine<CircuitDesigner>();
	ComponentDesigner* component_designer = app.getActiveEngine<ComponentDesigner>();

	if (component_designer) 
	{
		Component2D* activeComponent = component_designer->m_activeComponent.get();

		// Check that the active component exists. Close if not.
		if (activeComponent)
		{
			ImGui::Text(" Designator:\t  ");
			ImGui::SameLine();
			ImGui::InputText("##Designator", &activeComponent->designatorSym);
			ImGui::Text(" Type:\t  ");
			ImGui::SameLine();
			ImGui::InputText("##Equipment Type", &activeComponent->equipType);

			activeComponent->updateTextWithoutLabel();
		}

		if (ImGui::BeginChild("PortsChild", { 0, m_contentRegionSize.y / 4.5f }, true))
		{
			// Setup table.
			ImGui::BeginTable("Current ports", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp);
			ImGui::TableSetupColumn("Port Name", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("I/O Type      ", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Action      ", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();

			static std::vector<std::string> portPositions = { "Left", "Right", "Top", "Bottom" };
				int i = 0;
				for (std::shared_ptr<Port> port : activeComponent->ports)
				{
					// Table labels.
					char labelName[20];
					sprintf_s(labelName, "##N%d", i);
					char labelPos[20];
					sprintf_s(labelPos, "##P%d", i);
					char labelType[20];
					sprintf_s(labelType, "##T%d", i);
					char labelRemove[20];
					sprintf_s(labelRemove, "Remove##%d", i++);

					// Port entry in table.
					ImGui::TableNextRow();
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
					{
						activeComponent->removePort(port);
						ImGui::PopItemWidth();
						// Stop iterating through the ports if the port vector changes.
						break;
					}
					ImGui::PopItemWidth();
				}
				ImGui::EndTable();
		}
		ImGui::EndChild();

		//ImGui::PushID("CompEdChildData");
		if (ImGui::BeginChild("DataChild", { 0,0 }, true))
		{

			if (activeComponent)
			{
				std::unordered_map<std::string, std::string>& dataDict = activeComponent->dataDict;

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
					}
				}
			}
		}
		ImGui::EndChild();
	}
	if (design_engine) 
	{

		// Fetch all the component names.
		auto& numComponents = design_engine->m_circuit->m_components;
		auto& numCables = design_engine->m_circuit->m_cables;
		const char* componentNames[100];
		int numCom = 0;
		for (auto& key : numComponents)
		{
			componentNames[numCom] = key->designator->m_string.c_str();
			numCom++;
		}

		int numEquip = numCom;
		for (auto& key : numCables)
		{
			componentNames[numCom] = key->m_titleString.c_str();
			numCom++;
		}

		int numCable = numCom;

		// Fetch active elements.
		Component2D* activeComponent = design_engine->m_activeComponent.get();
		Cable* activeCable = design_engine->m_activeCable.get();

		std::string activeTitleString;

		// Check that the active component exists. Close if not.
		ImGui::PushID("CompGeneral");
		if (activeComponent)
		{

			ImGui::Text(" Designator:\t");
			ImGui::SameLine();
			ImGui::Text(activeComponent->designatorSym.c_str());

			activeTitleString = activeComponent->designator->m_string;

			if (ImGui::InputInt("##designatorIdx", &activeComponent->designatorIdx)) 
			{
				//std::string str = activeComponent->designatorSym + std::to_string();
				activeComponent->updateText();
			}

			ImGui::Text((std::string(" Type:\t  ") + activeComponent->equipType).c_str());
			//ImGui::SameLine();
			//ImGui::InputText("##Equipment Type", &activeComponent->equipType);
		}

		// ------------------------------- //
		//  C A B L E   S P E C I F I C S  //
		// ------------------------------- //

		if (activeCable)
		{
			// General info.
			ImGui::Text(" Name:\t");
			ImGui::SameLine();
			activeTitleString = activeCable->m_titleString;
			if (ImGui::InputText("##ComponentName", &activeCable->m_titleString))
			{
				//activeCable->m_title1->updateText(activeCable->m_titleString);
				//activeCable->m_title2->updateText(activeCable->m_titleString);
			}

			// --------------------------------------- //
			//  C A B L E   T Y P E   D R O P P I N G  //
			// --------------------------------------- //

			static glm::vec2 windowPadding = { 15.f, 15.f };
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, windowPadding);
			static float iconTextSeperation = 10.f;
			// Original setup.
			glm::vec2 iconSize = { 80, 80 };
			// Now adjust for the wrapped text.
			float childHeight = iconSize.y + s_textHeight + 2 * windowPadding.y + iconTextSeperation;
			float textX = ImGui::CalcTextSize(activeCable->m_cableType.c_str()).x;
			if (ImGui::BeginChild("CableDrop", {0.f, childHeight}, true, ImGuiWindowFlags_NoScrollbar))
			{
				if (activeCable->m_cableType.size())
				{
					float childContentRegionX = ImGui::GetContentRegionMax().x;
					ImGui::SetCursorPosX(childContentRegionX/2 - iconSize.x/2);
					ImGui::Image((void*)s_cableIcon, iconSize, { 0, 1 }, { 1, 0 });
					if (textX < childContentRegionX)
					{
						ImGui::SetCursorPosX(childContentRegionX/2 - textX/2);
					}
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + iconTextSeperation);
					ImGui::TextWrapped(activeCable->m_cableType.c_str());
				}
				else
				{
					ImGui::TextWrapped("No assigned type.");
				}
			}
			ImGui::PopStyleVar();
			ImGui::EndChild();

			// Drag & Drop files.
			LumenPayload payloadFiles(LumenPayloadType::String);
			payloadFiles.setDragAndDropTarget();
			if (payloadFiles.hasValidData()) design_engine->importCable(payloadFiles.getDataString());

			// Drag & Drop nodes.
			LumenPayload payloadNode(LumenPayloadType::YamlNode);
			payloadNode.setDragAndDropTarget();
			if (payloadNode.hasValidData())
			{
				bool checkForOverwrite = true;
				if (CircuitDesigner::s_engineUsedByCircuitEditor == design_engine) checkForOverwrite = false;
				design_engine->importCable(payloadNode.getDataYamlNode(), true, checkForOverwrite);
			}
		}

		// Get the current component as the initial selection for the data selection.
		if (equipmentSelector == -1) 
		{
			for (int num = 0; num < numCom; num++) 
			{
				equipmentSelector = num;
				if (componentNames[num] == activeTitleString) 
				{
					break;
				}
			}
		}

		const char* possibleInformation[100];

		int posKeys = 0;

		if (equipmentSelector < numEquip) 
		{
			for (auto& key : numComponents)
			{
				if (key->designator->m_string.c_str() == componentNames[equipmentSelector])
				{
					for (auto& [key2, val] : key->dataDict)
					{
						possibleInformation[posKeys] = key2.c_str();
						posKeys++;
					}
					break;
				}
			}
		}
		else 
		{
			for (auto& key : numCables)
			{
				if (key->m_titleString.c_str() == componentNames[equipmentSelector]) 
				{
					for (auto& [key2, val] : key->cableDict)
					{
						possibleInformation[posKeys] = key2.c_str();
						posKeys++;
					}
					break;
				}
			}
		}

		const char* additionalInformation[] = { "TierNumber", "BucketNumber", "MCC" };

		for (int i = 0; i < IM_ARRAYSIZE(additionalInformation); i++)
		{
			possibleInformation[posKeys] = additionalInformation[i];
			posKeys++;
		}

		ImGui::PopID();

	// --------------------- //
	//  D A T A   T A B L E  //
	// --------------------- //

	// Copy and paste dictiopnaries.
	if (activeComponent || activeCable)
	{
		if (ImGui::BeginChild("Copy&Paste", { 0.f, 35.f }, true))
		{
			// Copy button.
			if (ImGui::Button("Copy"))
			{
				if (activeComponent)
				{
					m_copiedDict = activeComponent->dataDict;
					m_copiedDictComponent = true;
					m_copiedDictCable = false;
					m_copiedDictFrom = activeComponent->designator->m_string;
					Lumen::getApp().pushNotification(NotificationType::Success, 2000, "Successfully copied data dictionary.", "Component Editor");
				}
				else if (activeCable)
				{
					m_copiedDict = activeCable->cableDict;
					m_copiedDictComponent = false;
					m_copiedDictCable = true;
					m_copiedDictFrom = activeCable->m_cableType;
					Lumen::getApp().pushNotification(NotificationType::Success, 2000, "Successfully copied data dictionary.", "Component Editor");
				}
			}
			ImGui::SameLine();

			// Check if paste should be available.
			bool canPaste = true;
			canPaste = (bool)m_copiedDict.size();
			canPaste = canPaste && ( (activeCable && m_copiedDictCable) || (activeComponent && m_copiedDictComponent) );

			// Paste button.
			if (!canPaste)
				ImGui::BeginDisabled();
			if (ImGui::Button("Paste"))
			{
				if		(activeComponent)  activeComponent->dataDict = m_copiedDict;
				else if (activeCable)	   activeCable->cableDict	 = m_copiedDict;
				Lumen::getApp().pushNotification(NotificationType::Success, 2000, "Successfully pasted data dictionary.", "Component Editor");
			}
			if (!canPaste)
				ImGui::EndDisabled();

			ImGui::SameLine();
			ImGui::Text("  Copied from:");
			ImGui::SameLine();
			ImGui::Text(m_copiedDictFrom.c_str());
		}
		ImGui::EndChild();
	}

	ImGui::PushID("CompEdChildData");
	if (ImGui::BeginChild("DataChild", { 0,0 }, true))
	{

		if (activeComponent || activeCable)
		{
			std::unordered_map<std::string, std::string>* dataDict = nullptr;
			if (activeComponent) dataDict = &activeComponent->dataDict;
			else if (activeCable) dataDict = &activeCable->cableDict;

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
				for (auto& [key, val] : *dataDict)
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

			const char* fromSelection[] = { "Circuit Database", "Motor Database", "CableData" };
			std::string from = "From(";
			std::string end = ")";

			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("From"))
			{
				// int* typeval2 = (int*)&dataDict;
				ImGui::Combo("Select Column##From", &fromSelector, buffer, dataDict->size());

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
				ImGui::Combo("Select Column##size", &sizeSelector, buffer, dataDict->size());
				// ImGui::Text(std::to_string(typeval3).c_str());
				if (ImGui::Button("Insert Size function"))
				{
					if (activeComponent)
					{
						activeComponent->dataDict[buffer[sizeSelector]] = "size()";
					}
					else
					{
						activeCable->cableDict[buffer[sizeSelector]] = "size()";
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
			std::string equipmentInfo;
			std::string pointer = "->";

			ImGui::SetNextItemOpen(false, ImGuiCond_Once);
			if (ImGui::CollapsingHeader("IF"))
			{
				ImGui::Combo("Select Column##IF", &ifSelector, buffer, dataDict->size());
				ImGui::Combo("Select Somponent##if2", &equipmentSelector, componentNames, numCom);
				ImGui::Combo("Select Variable To Compare##IF", &ifSelector2, possibleInformation, posKeys);
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

					ifString += componentNames[equipmentSelector] + pointer + possibleInformation[ifSelector2] + comma + comparatorSelection[comparatorSelector] + comma + comparisonValue + comma + trueStatement + comma + falseStatement + end;
					if (activeComponent)
					{
						activeComponent->dataDict[buffer[ifSelector]] = ifString;
					}
					else 
					{
						activeCable->cableDict[buffer[ifSelector]] = ifString;
					}
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
				ImGui::Combo("Select Column##Combine", &combineSelector, buffer, dataDict->size());
				if (ImGui::Combo("Select Variable##Combine", &combineSelectorVariable, possibleInformation, posKeys))
				{
					combineTextString += possibleInformation[combineSelectorVariable] + plusString;
				}
				ImGui::InputText("##Combine String", &combineTextString);
				if (ImGui::Button("Insert Combine function"))
				{
					if (combineTextString.substr(combineTextString.size() - 1, combineTextString.size()) == plusString) 
					{
						combineTextString = combineTextString.substr(0, combineTextString.size() - 1);
					}
					combineText += combineTextString + end;
					if (activeComponent)
					{
						activeComponent->dataDict[buffer[combineSelector]] = combineText;
					}
					else 
					{
						activeCable->cableDict[buffer[combineSelector]] = combineText;
					}
				}
			}
		}
	}
		ImGui::EndChild();
		ImGui::PopID();
	}
}

void ComponentEditor::onImGuiEnd()
{
	ImGui::End();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/