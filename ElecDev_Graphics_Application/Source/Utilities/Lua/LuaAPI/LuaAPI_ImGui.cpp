//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/ScriptGui/ScriptGui.h"
#include "Utilities/Lua/LuaInterpreter.h"
#include "Utilities/Lua/LuaAPI/LuaAPI.h"
#include "imgui/imgui.h"

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

// Used to format the dictionary so that imgui can plot a table.
inline int formatDictionary(std::map<std::string, std::vector<std::string>>& dict)
{
	// Find the longest vector.
	int length = 0;
	for (auto& [key, value] : dict)
	{
		int currentLength = value.size();
		if (currentLength > length)
			length = currentLength;
	}
	// Ensure all of the entries have the same length.
	for (auto& [key, value] : dict) 
	{
		int currentEntriesLength = value.size();
		while(currentEntriesLength < length) 
		{
			value.push_back("###DONOTDISPLAY");
			currentEntriesLength++;
		}
	}
	return length;
}


int lua_imgui_Text(lua_State* L) 
{
	// Get data.
	std::string text = lua_GetStringAndPop(L);

	// Render.
	ImGui::Text(text.c_str());

	return 1;
}

int lua_imgui_Button(lua_State* L)
{
	// Get data.
	glm::vec2 size(0.f);
	lua_GetTableAndPop<float>(L, &size[0], 2);
	std::string text = lua_GetStringAndPop(L);

	// Render.
	if (ImGui::Button(text.c_str(), size))
	{
		// Press occurred.
		std::string msg = "[Button] " + text + " : Pressed.";
		Lumen::getActiveScriptGui()->callbackMessage(msg);
		return 1;
	}

	// Press did not occur.
	return 0;
}

int lua_imgui_CloseButton(lua_State* L) 
{
	// Get data.
	glm::vec2 size(0.f);
	lua_GetTableAndPop<float>(L, &size[0], 2);
	std::string text = lua_GetStringAndPop(L);

	// Render.
	if (ImGui::Button(text.c_str(), size))
	{
		Application& app = Lumen::getApp();
		// Press occurred.
		std::string msg = "[CloseButton] " + text + " : Pressed.";
		ScriptGui* gui = Lumen::getActiveScriptGui();
		gui->callbackMessage(msg);
		app.queueWindowPop(gui->m_layer);
		return 1;
	}

	// Press did not occur.
	return 0;
}

int lua_imgui_ClearAwaitScriptButton(lua_State* L) 
{
	// Get data.
	glm::vec2 size(0.f);
	lua_GetTableAndPop<float>(L, &size[0], 2);
	std::string text = lua_GetStringAndPop(L);

	// Render.
	if (ImGui::Button(text.c_str(), size))
	{
		Application& app = Lumen::getApp();
		// Press occurred.
		std::string msg = "[ClearAwaitScriptButton] " + text + " : Pressed.";
		ScriptGui* gui = Lumen::getActiveScriptGui();
		gui->callbackMessage(msg);
		gui->m_script.clear();
		gui->awaitNewScript();
		return 1;
	}

	// Press did not occur.
	return 0;
}

int lua_imgui_SameLine(lua_State* L) 
{
	// Get data.
	float offset = lua_GetNumberAndPop<float>(L);

	// Render.
	ImGui::SameLine(offset);

	return 1;
}

int lua_imgui_Separator(lua_State* L) 
{
	// Render.
	ImGui::Separator();

	return 1;
}

int lua_imgui_Combo(lua_State* L) 
{
	// Contains the state for all of the comboboxes.
	static std::unordered_map<std::string, int> comboItems;  // How to clear this?

	// Get data.
	int maxHeightInItems = lua_GetNumberAndPop<int>(L);
	std::vector<std::string> items;
	lua_GetStringTableAndPop(L, items);
	int initialItem = lua_GetNumberAndPop<int>(L);
	std::string label = lua_GetStringAndPop(L);

	int itemsCharLength = 0;
	for (auto& item : items) itemsCharLength += item.size();

	// Create zero seperated string.
	char* memory = (char*)calloc(itemsCharLength + items.size() + 1, sizeof(char));
	const char* zeroSeperatedString = (const char*)memory;
	char nullTerminator = '\0';

	int index = 0;
	for (auto& item : items)
	{
		for (auto& c : item) 
			memory[index++] = c;
		memory[index++] = nullTerminator;
	}
	memory[index++] = nullTerminator;

	// Store item. This does not store the item if it already exists.
	comboItems.insert({label, initialItem });
	// Get item.
	int* currentItem = &comboItems[label];

	// Render.
	bool selected = false;
	if (ImGui::Combo(label.c_str(), currentItem, zeroSeperatedString, maxHeightInItems))
	{
		std::string msg = "[Combo] " + label + " : Item " + items[*currentItem] + ".";
		Lumen::getActiveScriptGui()->callbackMessage(msg);
		selected = true;
	}
	free((void*)memory);
	return selected;
}

int lua_imgui_Checkbox(lua_State* L) 
{
	// Contains the state for all of the checkboxes.
	static std::unordered_map<std::string, bool> checkboxStates;  // How to clear this?

	// Get data.
	bool state = lua_GetBooleanAndPop(L);
	std::string label = lua_GetStringAndPop(L);

	// Store item. This does not store the item if it already exists.
	checkboxStates.insert({ label,  state});
	bool* currentState = &checkboxStates[label];

	// Render.
	if (ImGui::Checkbox(label.c_str(), currentState)) 
	{
		std::string msg = "[Checkbox] " + label + " : " + std::to_string(*currentState) + ".";
		Lumen::getActiveScriptGui()->callbackMessage(msg);
		return 1;
	}

	return 0;
}

int lua_imgui_InputText(lua_State* L) 
{
	// Contains the state for all of the input text.
	static std::unordered_map<std::string, std::string> inputTextEntries;  // How to clear this?

	// Get data.
	std::string initialText = lua_GetStringAndPop(L);
	std::string label = lua_GetStringAndPop(L);

	// Store item. This does not store the item if it already exists.
	inputTextEntries.insert({ label, initialText });
	// Get data.
	std::string* currentEntry = &inputTextEntries[label];
	
	// Render.
	if (ImGui::InputText(label.c_str(), currentEntry)) 
	{
		std::string msg = "[InputText] " + label + " : " + *currentEntry;
		Lumen::getActiveScriptGui()->callbackMessage(msg);
		return 1;
	}

	return 0;
}

int lua_imgui_Table(lua_State* L) 
{
	static std::unordered_map<
		std::string,
		std::map<std::string, std::vector<std::string>>
	> tableData;

	// Get data.
	std::map<std::string, std::vector<std::string>> dict;
	lua_GetDictAndPop(L, dict);
	float height = lua_GetNumberAndPop<float>(L);
	std::string label = lua_GetStringAndPop(L);

	// Store and get data.
	tableData.insert({label, dict});
	std::map<std::string, std::vector<std::string>>& currentDict = tableData[label];

	// Render table.
	ImGui::PushID(label.c_str());
	if (ImGui::BeginChild("Child", {0.f, height}, true))
	{
		// Setup table.
		ImGui::BeginTable("Table", currentDict.size(), ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
		for (auto& [key, values] : currentDict)
		{
			ImGui::TableSetupColumn(key.c_str(), ImGuiTableColumnFlags_WidthStretch);
		}
		ImGui::TableHeadersRow();
		ImGui::TableNextRow();

		// Populate values.
		int rows = formatDictionary(currentDict);
		int columns = currentDict.size();
		// Iterate over rows.
		for (int r = 0; r < rows; r++)
		{
			if (r)
				ImGui::TableNextRow();
			ImGui::PushID(std::to_string(r).c_str());
			ImGui::TableSetColumnIndex(0);
			// Iterate over columns.
			int column = 0;
			for (auto& [key, value] : currentDict)
			{
				if (value[r] != "###DONOTDISPLAY")
				{
					ImGui::PushID(key.c_str());
					ImGui::PushItemWidth(-1);
					if (ImGui::InputText("##Input", &value[r])) 
					{
						std::string msg = "[Table] " + label + " : [Key] " + key + " [Value " + std::to_string(r) + "] : " + value[r];
						Lumen::getActiveScriptGui()->callbackMessage(msg);
					}
					ImGui::PopItemWidth();
					ImGui::PopID();
				}
				// Prevent empty last row.
				if (column < columns-1)
					ImGui::TableNextColumn();
				column++;
			}
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
	ImGui::EndChild();
	ImGui::PopID();

	return 1;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//