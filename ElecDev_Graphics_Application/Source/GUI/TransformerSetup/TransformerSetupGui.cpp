#include "TransformerSetupGui.h"
#include <unordered_map>

TransformerSetupGui::TransformerSetupGui(const std::string& name, std::unordered_map<std::string, std::string[2]>& dataDict, int imguiWindowFlags) :LumenWebsocketWindow(name, imguiWindowFlags),
m_dataDict(dataDict)
{
}

void TransformerSetupGui::onImGuiBegin()
{
}

void TransformerSetupGui::onImGuiRender()
{
	using namespace ImGui;
	Text("Transformer Setup");
	BeginChild("Automatic Transformer Assignment");
	Combo("Max Transformer Size", &transformerSizeIdx, transformerSizes, IM_ARRAYSIZE(transformerSizes));
	SameLine();
	InputInt("Utilization Target", &utilizationTarget);
	BeginTable("##tab1", 3);
	EndTable();
	EndChild();
}

void TransformerSetupGui::onImGuiEnd()
{
}
