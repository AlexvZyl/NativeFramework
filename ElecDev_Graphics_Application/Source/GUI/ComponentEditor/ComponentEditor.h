#pragma once

//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "Application/Windows/LumenWindow.h"
#include <unordered_map>
#include "yaml-cpp/yaml.h"

//=======================================================================================================================================//
// Component Editor.																													 //
//=======================================================================================================================================//

class Cable;
class Text;
class CommandLog;

class ComponentEditor : public LumenWindow
{
public:

	// Constructor.
	ComponentEditor(std::string name, int windowFlags = 0);
	
	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

private:
	//Vars saving original data while edit is in progress
	//We can reuse these, as we can only have one active edit at a time.
	std::string strBeforeEdit;

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

	inline static unsigned s_cableIcon = 0;
	inline static bool s_iconCreated = false;
	inline static float s_textHeight = 0;

	// Allows the user to copy and paste the current dictionary.
	inline static std::unordered_map<std::string, std::string> m_copiedDict;
	inline static bool m_copiedDictCable = false;
	inline static bool m_copiedDictComponent = false;
	inline static std::string m_copiedDictFrom = "";

	void inputTextWithLog(const char* label, Text* textToUpdate, CommandLog* log);
	void inputTextWithLog(const char* label, std::string& stringToUpdate, CommandLog* log);
};

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//