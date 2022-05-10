#include "LumenPayload.h"

LumenPayload::LumenPayload(LumenPayloadType type) 
	: m_type(type)
{}

void LumenPayload::setData(void* data, size_t size)
{
	m_dataPtr = data;
	m_dataSize = size;
}

void LumenPayload::setData(const std::string& data) 
{
	m_dataPtr = (void*)&data;
	m_dataSize = data.size();
}

std::string LumenPayload::getDataString() 
{
	return std::string((const char*)m_dataPtr);
}

void LumenPayload::setDragAndDropSource()
{
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload(getTypeCString(), m_dataPtr, m_dataSize, m_imguiCond);
		ImGui::EndDragDropSource();
	}
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

std::tuple<void*, size_t> LumenPayload::getData()
{
	return { m_dataPtr, m_dataSize };
}

const char* LumenPayload::getTypeCString() 
{
	switch (m_type)
	{
	case LumenPayloadType::FilePath:
		return "FILE_PATH";
		break;

	case LumenPayloadType::YamlNode:
		return "YAML_NODE";
		break;

	default:
		LUMEN_LOG_WARN("Unknown payload type.", "");
		return "";
	}
}

bool LumenPayload::hasValidData() 
{
	return m_dataPtr && m_dataSize;
}