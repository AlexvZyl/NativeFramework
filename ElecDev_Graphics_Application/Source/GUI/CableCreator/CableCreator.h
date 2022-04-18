#pragma once

//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "Application/LumenWindow/LumenWindow.h"
#include <unordered_map>
#include <filesystem>
#include <string>

//=======================================================================================================================================//
// Component Editor.																													 //
//=======================================================================================================================================//

class CableCreator : public LumenWindow
{
public:

	// Constructor.
	CableCreator(std::string name, int windowFlags = 0);

	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

	// Cable properties.
	glm::vec4 m_cableColor = {0.f, 0.f, 0.f, 1.f};
	std::string m_cableName = "";
	std::unordered_map<std::string, std::string> m_dataDict;

	// Set the data that is contained in the provided file.
	void setCable(const std::filesystem::path& path);

private:

	// Data for creating cable properties.
	int fromSelector = 0;
	int databaseSelector = 0;
	int sizeSelector = 0;
	int ifRowSelector = 0;
	int comparatorSelector = 0;
	int ifSelector = 0;
	int ifSelector2 = 0;
	int equipmentSelector = -1;
	int combineSelector = 0;
	int combineSelectorVariable = 0;
	const char* comparatorSelection[6] = { "==", "!=", "<=", ">=", "<", ">" };
	std::string comparisonValue = "0";
	std::string trueStatement = "True";
	std::string falseStatement = "False";
	std::string combineTextString = "";
	bool addingPort = false;
	std::string newName = "Untitled";
	int newType = 2;
	int newPos = 0;

	std::string m_entryToAdd = "";

	// Saves the cable to file.
	void serialiseCable(const std::filesystem::path& path);
};

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//