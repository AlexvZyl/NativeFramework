//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "LumenWebsocketWindow.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Utilities/Lua/LuaInterpreter.h"
#include "Utilities/Logger/Logger.h"

using boost_ip_address = boost::asio::ip::address;

//==============================================================================================================================================//
//  General.																																	//
//==============================================================================================================================================//

LumenWebsocketWindow::LumenWebsocketWindow(const std::string& name, int windowFlags)
    : LumenWindow(name, windowFlags)
{ }

LumenWebsocketWindow::~LumenWebsocketWindow()
{
    // Close websocket.
    boost_error_code ec;
    boost::beast::websocket::close_reason cr("Lumen Websocket Window shutdown.");
    m_webSocket->close(cr, ec);
    m_webSocket.reset();
    m_ioContext.reset();

    // Check for errors.
    if (ec)
    {
        LUMEN_LOG_ERROR("On websocket close: " + ec.message(), "Lumen Websocket Window");
        ec.clear();
    }
}

//==============================================================================================================================================//
//  Websocket.																																	//
//==============================================================================================================================================//

void LumenWebsocketWindow::connectWebSocket(const std::string& host, const std::string& port)
{
    // Create websocket.
    boost_error_code ec;
    m_ioContext = std::make_unique<asio_ioc>();
    asio_endpoint ep(asio::ip::make_address(host, ec), std::stoi(port));
    asio_websocket ws(*m_ioContext.get());

    // Check for errors.
    if (ec)
    {
        LUMEN_LOG_ERROR("On address creation: " + ec.message(), "Lumen Websocket Window");
        ec.clear();
    }

    // Connect.
    ws.connect(ep, ec);

    // Check for errors.
    if (ec)
    {
        LUMEN_LOG_ERROR("On websocket connection: " + ec.message(), "Lumen Websocket Window");
        ec.clear();
    }

    m_webSocket = std::make_unique<basic_boost_websocket>(std::move(ws));
    m_webSocket->handshake(host, port, ec);

    // Check for errors.
    if (ec)
    {
        LUMEN_LOG_ERROR("On websocket handshake: " + ec.message(), "Lumen Websocket Window");
        ec.clear();
    }

    // Check if the socket is open.
    if (!m_webSocket->is_open())
    {
        LUMEN_LOG_ERROR("Socket is not open on connection.", "Lumen Websocket Window");
    }
}

void LumenWebsocketWindow::callbackMessage(const std::string& message)
{
    boost_error_code ec;

    // Check if the socket is open.
    if (!m_webSocket->is_open())
    {
        LUMEN_LOG_ERROR("Socket is not open on callback.", "Lumen Websocket Window");
        return;
    }

    // Write to socket.
    boost::asio::const_buffer data(message.data(), message.size());
    size_t bytes = m_webSocket->write(data, ec);

    // Check if data sent.
    if (!bytes)
    {
        LUMEN_LOG_ERROR("On callback: No data sent.", "Lumen Websocket Window");
    }

    // Check for errors.
    if (ec)
    {
        LUMEN_LOG_ERROR("On callback: " + ec.message(), "Lumen Websocket Window");
        ec.clear();
    }
}

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//