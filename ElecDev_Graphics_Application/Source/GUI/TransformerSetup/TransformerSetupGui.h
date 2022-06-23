#pragma once
#include "Application/Windows/LumenWebsocketWindow.h"
class TransformerSetupGui :
    public LumenWebsocketWindow
{
private:
    std::unordered_map<std::string, std::string[2]> m_dataDict;
    const char* const transformerSizes[2] = {"2500", "2000"};
    int transformerSizeIdx = 0;
    int utilizationTarget = -1;

public:
    TransformerSetupGui(const std::string& name, std::unordered_map<std::string, std::string[2]>& dataDict, int imguiWindowFlags = 0);

    virtual void onImGuiBegin() override;
    virtual void onImGuiRender() override;
    virtual void onImGuiEnd() override;

};

