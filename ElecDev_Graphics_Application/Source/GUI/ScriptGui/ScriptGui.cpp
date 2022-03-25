//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "ScriptGui.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Utilities/Lua/LuaInterpreter.h"
#include "Utilities/Logger/Logger.h"

using boost_ip_address = boost::asio::ip::address;

//==============================================================================================================================================//
//  General.																																	//
//==============================================================================================================================================//

ScriptGui::ScriptGui(std::string name, int windowFlags)
    : GuiElementCore(name, windowFlags)
{
    m_luaState = lua_CreateNewLuaState();
}

ScriptGui::~ScriptGui() 
{
    // Close websocket.
    boost_error_code ec;
    boost::beast::websocket::close_reason cr("Lumen GUI shutdown.");
    m_webSocket->close(cr, ec);

    // Check for errors.
    if (ec)
    {
        LUMEN_LOG_ERROR("On websocket close: " + ec.message(), "Script Gui Socket");
        ec.clear();
    }

    // Close Lua.
    lua_close(m_luaState);
}

void ScriptGui::setSctipt(std::string& script) 
{
    m_script = script;
}

void ScriptGui::begin()
{
    Lumen::setAvtiveScriptGui(this);
    ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void ScriptGui::onRender()
{
    lua_ExecuteScript(m_luaState, m_script.c_str());
}

void ScriptGui::end()
{
    ImGui::End();
}

//==============================================================================================================================================//
//  Websocket.																																	//
//==============================================================================================================================================//

void ScriptGui::connectWebSocket(std::string& host, std::string& port)
{
    // Create websocket.
    boost_error_code ec;
    m_ioContext = std::make_unique<asio_ioc>();
    asio_endpoint ep(asio::ip::make_address(host, ec), std::stoi(port));
    asio_websocket ws(*m_ioContext.get());

    // Check for errors.
    if (ec)
    {
        LUMEN_LOG_ERROR("On address creation: " + ec.message(), "Script Gui Socket");
        ec.clear();
    }

    // Connect.
    ws.connect(ep, ec);

    // Check for errors.
    if (ec)
    {
        LUMEN_LOG_ERROR("On websocket connection: " + ec.message(), "Script Gui Socket");
        ec.clear();
    }

    m_webSocket = std::make_unique<basic_boost_websocket>(std::move(ws));
    m_webSocket->handshake(host, port, ec);

    // Check for errors.
    if (ec)
    {
        LUMEN_LOG_ERROR("On websocket handshake: " + ec.message(), "Script Gui Socket");
        ec.clear();
    }

    // Check if the socket is open.
    if (!m_webSocket->is_open())
    {
        LUMEN_LOG_ERROR("Socket is not open on connection.", "Script Gui Socket");
    }
}

void ScriptGui::callbackMessage(std::string& message) 
{
    boost_error_code ec;

    // Check if the socket is open.
    if (!m_webSocket->is_open())
    {
        LUMEN_LOG_ERROR("Socket is not open on callback.", "Script Gui Socket");
        return;
    }

    // Write to socket.
    boost::asio::const_buffer data(message.data(), message.size());
    size_t bytes = m_webSocket->write(data, ec);

    // Check if data sent.
    if (!bytes)
    {
        LUMEN_LOG_ERROR("On callback: No data sent.", "Script Gui Socket");
    }

    // Check for errors.
    if (ec)
    {
        LUMEN_LOG_ERROR("On callback: " + ec.message(), "Script Gui Socket");
        ec.clear();
    }
}

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//