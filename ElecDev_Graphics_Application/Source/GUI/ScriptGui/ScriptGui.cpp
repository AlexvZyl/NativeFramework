#include "ScriptGui.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "Utilities/Lua/LuaInterpreter.h"
#include "Utilities/Logger/Logger.h"

ScriptGui::ScriptGui(const std::string& name, int windowFlags)
    : LumenWebsocketWindow(name, windowFlags)
{
    m_luaState = lua_CreateNewLuaState();
}

ScriptGui::~ScriptGui() 
{
    lua_close(m_luaState);
}

void ScriptGui::setSctipt(std::string script) 
{
    m_script = std::move(script);
}

void ScriptGui::onImGuiBegin()
{
    Lumen::setAvtiveScriptGui(this);
    ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
}

void ScriptGui::onImGuiRender()
{
    lua_ExecuteScript(m_luaState, m_script.c_str());
}

void ScriptGui::onImGuiEnd()
{
    ImGui::End();
}

void ScriptGui::awaitNewScript() 
{
    m_listenerThread = std::thread([&]() 
        {
            bool listen = true;
            while (listen)
            {
                boost::beast::flat_buffer buffer;
                m_webSocket->read(buffer);
                if (!buffer.size()) continue;
                std::string input = boost::beast::buffers_to_string(buffer.cdata());
                LUMEN_LOG_ERROR("Received new script.", "DEBUG");
                m_script = input;
                listen = false;
            }
            m_listenerThread.join();
        });
}