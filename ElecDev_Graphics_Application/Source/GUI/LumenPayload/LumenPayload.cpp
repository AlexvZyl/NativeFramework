#include "LumenPayload.h"

LumenPayload::LumenPayload(LumenPayloadType type) 
	: m_type(type)
{}

void LumenPayload::setDragAndDropSource(void* data, size_t size)
{
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload(getTypeCString(), data, size, m_imguiCond);
		ImGui::EndDragDropSource();
	}
}

void LumenPayload::setDragAndDropSource(const char* data, size_t size)
{
	setDragAndDropSource((void*)data, size);
}

void LumenPayload::setDragAndDropSource(const std::string& data) 
{
	setDragAndDropSource(data.c_str(), data.size() + 1);
}

void LumenPayload::setDragAndDropSource(const std::filesystem::directory_entry& path)
{
	setDragAndDropSource(path.path().string());
}

void LumenPayload::setDragAndDropSource(const YAML::Node& node) 
{
	setDragAndDropSource((void*)&node, sizeof(node));
}

void LumenPayload::setDragAndDropTarget() 
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(getTypeCString()))
		{
			m_dataPtr = payload->Data;
			m_dataSize = payload->DataSize;
		}
		ImGui::EndDragDropTarget();	
	}
}

 void LumenPayload::setImGuiCond(int cond)
{
	m_imguiCond = cond;
}

std::tuple<void*, size_t> LumenPayload::getDataRaw()
{
	return { m_dataPtr, m_dataSize };
}

std::string LumenPayload::getDataString()
{
	return std::string(getDataCString());
}

const char* LumenPayload::getDataCString()
{
	return static_cast<const char*>(m_dataPtr);
}

YAML::Node LumenPayload::getDataYamlNode() 
{
	return *static_cast<YAML::Node*>(m_dataPtr);
}

const char* LumenPayload::getTypeCString() 
{
	switch (m_type)
	{
	case LumenPayloadType::String:
		return "STRING";
		break;

	case LumenPayloadType::YamlNode:
		return "YAML_NODE";
		break;

	default:
		LUMEN_LOG_WARN("Unknown payload type.", "LumenPayload");
		return "";
	}
}

bool LumenPayload::hasValidData() 
{
	return m_dataPtr && m_dataSize;
}