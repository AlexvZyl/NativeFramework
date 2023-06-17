#pragma once

// Includes.
#include "imgui/imgui.h"
#include "imgui_internal.h"
#include "Utilities/Logger/Logger.h"
#include <filesystem>
#include "yaml-cpp/yaml.h"

// Payload types.
enum class LumenPayloadType 
{
	String,
	YamlNode,
};

class LumenPayload 
{
public:

	// Constructor.
	LumenPayload(LumenPayloadType type);
	// Destructor.
	inline ~LumenPayload() = default;

	// Set the imgui cond used when calling the function.
	void setImGuiCond(int cond);

	// Set the data.
	void setDragAndDropSource(void* data, size_t size);
	void setDragAndDropSource(const char* data, size_t size);
	void setDragAndDropSource(const std::string& data);
	void setDragAndDropSource(const std::filesystem::directory_entry& path);
	void setDragAndDropSource(const YAML::Node& node);

	// Set the payload as a target.
	void setDragAndDropTarget();

	// Check if the payload has valid data.
	bool hasValidData();

	// Get the data.
	const char* getDataCString();
	std::string getDataString();
	YAML::Node getDataYamlNode();
	std::tuple<void*, size_t> getDataRaw();
	template<typename T>
	T getData() 
	{
		return *static_cast<T*>(m_dataPtr);
	}

private:

	// Get the type as a string.
	const char* getTypeCString();

	LumenPayloadType m_type;
	void* m_dataPtr = nullptr;
	size_t m_dataSize = NULL;
	int m_imguiCond = ImGuiCond_Once;
};