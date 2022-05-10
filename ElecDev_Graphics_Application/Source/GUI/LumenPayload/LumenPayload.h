#pragma once

// Includes.
#include "imgui/imgui.h"
#include "imgui_internal.h"
#include "Utilities/Logger/Logger.h"

// Payload types.
enum class LumenPayloadType 
{
	FilePath,
	YamlNode,
};

class LumenPayload 
{
public:

	// Constructor.
	LumenPayload(LumenPayloadType type);

	// Set data manually.
	void setData(void* data, size_t size);
	void setData(const std::string& data);

	// Set the payload to be dragged.
	void setDragAndDropSource();

	// Set the payload to be dragged.
	void setDragAndDropTarget();

	// Set the imgui cond used when calling the function.
	void setImGuiCond(int cond);

	// Get the data manually.
	std::tuple<void*, size_t> getData();

	// Get the type as a string.
	const char* getTypeCString();

	// Check if the payload has valid data.
	bool hasValidData();

	// Get the data based on the type.
	std::string getDataString();

private:

	LumenPayloadType m_type;
	void* m_dataPtr = nullptr;
	size_t m_dataSize = NULL;
	int m_imguiCond = ImGuiCond_Once;
};