//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "CableCreator.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Cable.h"
#include "Resources/ResourceHandler.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include <fstream>
#include "Utilities/Serialisation/Serialiser.h"

//=======================================================================================================================================//
// Circuit editor.																														 //
//=======================================================================================================================================//

CableCreator::CableCreator(std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags)
{
	// Create default dectionary.
	m_dataDict.insert({"FromTagNumber", "From(Circuit Database)"});
	m_dataDict.insert({"ToTagNumber", "From(Circuit Database)"});
	m_dataDict.insert({"From", "From(Circuit Database)"});
	m_dataDict.insert({"To", "From(Circuit Database)"});
	m_dataDict.insert({"Description", "From(Circuit Database)"});
	m_dataDict.insert({"Voltage", "From(Circuit Database)"});
	m_dataDict.insert({"Circuit", "From(Circuit Database)"});
	m_dataDict.insert({"CableRating", "From(Circuit Database)"});
	m_dataDict.insert({"CableIsolation", "From(Circuit Database)"});
	m_dataDict.insert({"CableSuffix", "From(Circuit Database)"});
	m_dataDict.insert({"CableLength", "From(Circuit Database)"});
	m_dataDict.insert({"CableTAG", "From(Circuit Database)"});
	m_dataDict.insert({"IndoorTermCount", "From(Circuit Database)"});
	m_dataDict.insert({"OutdoorTermCount", "From(Circuit Database)"});
	m_dataDict.insert({"CableSize", "size()"});
	m_dataDict.insert({"CoreCount", "From(Circuit Database)"});
	m_dataDict.insert({"Metal", "From(Circuit Database)"});
	m_dataDict.insert({"DBRef", "From(Circuit Database)"});
	m_dataDict.insert({"CableDiameter", "From(Circuit Database)"});
	m_dataDict.insert({ "CableMass", "From(Circuit Database)" });
}

void CableCreator::begin()
{
	ImGui::SetNextWindowSize({500, 700}, ImGuiCond_Once);
	glm::vec2 vpSize = ImGui::GetMainViewport()->Size;
	ImGui::SetNextWindowPos({ vpSize.x / 2, vpSize.y / 2 }, ImGuiCond_Once, {0.5f, 0.5f});
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void CableCreator::onRender()
{
	// ------------------------- //
	//  G E N E R A L   D A T A  //
	// ------------------------- //

	ImGui::Text("Cable Name:");
	ImGui::SameLine();
	ImGui::InputText("##CableCreatorName", &m_cableName);

	ImGui::Text("Cable color:  ");
	ImGui::SameLine();
	ImGui::ColorEdit4("##CableColorPicker", &m_cableColor[0]);

	if (ImGui::Button("Save", { m_contentRegionSize.x, 0.f }))
	{
		std::filesystem::path path = selectFile("Lumen Save File", "", m_cableName, "Save");
		if (path.string().size())
			serialiseCable(path);
	}


	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Buffer.
	const char* buffer[100];
	int numKeys = 0;
	for (auto& [key, val] : m_dataDict)
	{
		buffer[numKeys] = key.c_str();
		numKeys++;
	}

	// Possible information.
	/*const char* possibleInformation[100];
	int posKeys = 0;
	if (equipmentSelector < numEquip) 
	{
		for (auto& key : numComponents)
		{
			if (key->titleString.c_str() == componentNames[equipmentSelector]) 
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
	}*/
	const char* additionalInformation[] = { "TierNumber", "BucketNumber", "MCC" };
	/*for (int i = 0; i < IM_ARRAYSIZE(additionalInformation); i++)
	{
		possibleInformation[posKeys] = additionalInformation[i];
		posKeys++;
	}*/


	// --------------------- //
	//  D I C T I O N A R Y  //
	// --------------------- //

	std::vector<std::string> toRemoveKeys;
	if (ImGui::BeginChild("DataDict", {0.f, 0.f}, true))
	{
		// Add dict entry.
		static std::string entryToAdd;
		ImGui::Text("Add an attribute to the dictionary:");
		ImGui::InputText("##DictEntry", &entryToAdd);
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			m_dataDict.insert({ entryToAdd, "From(Circuit Database)" });
			entryToAdd = "";
		}

		// Setup table.
		ImGui::BeginTable("CableDictTable", 3,   ImGuiTableFlags_Resizable
											   | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp
											   | ImGuiTableFlags_Borders);
		ImGui::TableSetupColumn("Attribute", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Function", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();
		ImGui::TableNextRow();

		int entryIndex = 0;
		for (auto& [key, value] : m_dataDict)
		{
			ImGui::PushID(std::to_string(entryIndex).c_str());
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(key.c_str());
			ImGui::TableSetColumnIndex(1);
			ImGui::InputText("##Input", &value);
			ImGui::TableSetColumnIndex(2);
			std::string buttonID = "Remove##" + std::to_string(entryIndex++);
			if (ImGui::Button(buttonID.c_str()))
			{
				toRemoveKeys.push_back(key);
			}
			ImGui::TableNextRow();
			ImGui::PopID();
		}
		ImGui::EndTable();

		// Remove entries.
		for (auto& key : toRemoveKeys)
			m_dataDict.erase(key);
		toRemoveKeys.clear();

		// --------------------- //
		//     FROM SELECTION    //
		// --------------------- //

		const char* fromSelection[] = { "Circuit Database", "Motor Database", "CableData" };
		std::string from = "From(";
		std::string end = ")";

		ImGui::SetNextItemOpen(false, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("From"))
		{
			ImGui::Combo("Select Column##From", &fromSelector, buffer, m_dataDict.size());
			ImGui::Combo("Select Database##From2", &databaseSelector, fromSelection, IM_ARRAYSIZE(fromSelection));
			if (ImGui::Button("Insert From function"))
			{
				from += fromSelection[databaseSelector] + end;
				m_dataDict[buffer[fromSelector]] = from;
			}
		}

		// ------------ //
		//     SIZE     //
		// ------------ //

		ImGui::SetNextItemOpen(false, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Size"))
		{
			ImGui::Combo("Select Column##size", &sizeSelector, buffer, m_dataDict.size());
			if (ImGui::Button("Insert Size function"))
				m_dataDict[buffer[sizeSelector]] = "size()";
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
			ImGui::Combo("Select Column##IF", &ifSelector, buffer, m_dataDict.size());
			//ImGui::Combo("Select Somponent##if2", &equipmentSelector, componentNames, numCom);
			ImGui::Combo("Select Variable To Compare##IF", &ifSelector2, buffer, m_dataDict.size());
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

				ifString += pointer + buffer[ifSelector2] + comma + comparatorSelection[comparatorSelector] + comma + comparisonValue + comma + trueStatement + comma + falseStatement + end;
				m_dataDict[buffer[ifSelector]] = ifString;
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
			ImGui::Combo("Select Column##Combine", &combineSelector, buffer, m_dataDict.size());
			if (ImGui::Combo("Select Variable##Combine", &combineSelectorVariable, buffer, m_dataDict.size()))
			{
				combineTextString += buffer[combineSelectorVariable] + plusString;
			}
			ImGui::InputText("##Combine String", &combineTextString);
			if (ImGui::Button("Insert Combine function"))
			{
				if (combineTextString.substr(combineTextString.size() - 1, combineTextString.size()) == plusString) 
				{
					combineTextString = combineTextString.substr(0, combineTextString.size() - 1);
				}
				combineText += combineTextString + end;
				m_dataDict[buffer[combineSelector]] = combineText;
			}
		}
	}
	ImGui::EndChild();
}

void CableCreator::end()
{
	ImGui::End();
}

void CableCreator::serialiseCable(const std::filesystem::path & path)
{
	// Change the cable name to the file name supplied.
	if (path.filename().string().size())
		m_cableName = path.filename().stem().string();

	// Create yaml file.
	YAML::Emitter yamlEmitter;
	// General data.
	yamlEmitter << YAML::BeginMap;
	yamlEmitter << YAML::Key << "Lumen File Info" << YAML::Value;
	yamlEmitter << YAML::BeginMap;
	yamlEmitter << YAML::Key << "Version" << YAML::Value << "0.0.1";
	yamlEmitter << YAML::Key << "Type" << YAML::Value << "Cable";
	yamlEmitter << YAML::EndMap;

	// Cable data.
	yamlEmitter << YAML::Key << "Cable" << YAML::Value;
	yamlEmitter << YAML::BeginMap;
	yamlEmitter << YAML::Key << "Label" << YAML::Value << m_cableName;
	yamlEmitter << YAML::Key << "Color" << YAML::Value << m_cableColor;
	yamlEmitter << YAML::Key << "Dictionary" << YAML::Value << m_dataDict;
	yamlEmitter << YAML::EndMap;

	// End yaml file.
	yamlEmitter << YAML::EndMap;

	// If a directory was received and not a file, create a file from the cable label.
	std::string saveLocation = path.string();
	if (!path.filename().string().size())
	{
		saveLocation += m_cableName + ".lmcb";
	}
	// Check if a file extension was supplied.
	else if (path.filename().extension().string() != ".lmcb")
	{
		saveLocation += ".lmcb";
	}

	// Save the yaml file.
	std::ofstream yamlStream;
	yamlStream.open(saveLocation);
	yamlStream << yamlEmitter.c_str();
	yamlStream.close();
}

void CableCreator::setCable(const std::filesystem::path& path)
{
	YAML::Node node = YAML::LoadFile(path.string())["Cable"];

	m_cableName = node["Label"].as<std::string>();
	m_cableColor = { node["Color"][0].as<float>(), node["Color"][1].as<float>(), node["Color"][2].as<float>(), node["Color"][3].as<float>() };

	m_dataDict.clear();
	for (const auto& dictPair : node["Dictionary"])
	{
		m_dataDict.insert({dictPair.first.as<std::string>(), dictPair.second.as<std::string>()});
	}
}

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//