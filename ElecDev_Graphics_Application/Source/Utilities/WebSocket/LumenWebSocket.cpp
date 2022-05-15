//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/ScriptGui/ScriptGui.h"
#include "BoostWebsocket.h"
#include "LumenWebSocket.h"
#include "Application/ApplicationTemplates.h"
#include <iostream>
#include <thread>
#include "External/Misc/ConsoleColor.h"
#include "Utilities/Platform/Thread.h"
#include "Application/Application.h"
#include "Utilities/Lua/LuaInterpreter.h"
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

LumenWebSocket::LumenWebSocket(const std::string& ip)
{
	// Create connection and context.
	m_socketAddress = std::make_unique<boost_ip_address>(boost::asio::ip::make_address(ip));

	// Start listener thread.
	m_listenerThread = std::thread(&LumenWebSocket::listener, this);
}

LumenWebSocket::~LumenWebSocket()
{
	lumenTerminateThread(m_listenerThread);
}

void LumenWebSocket::listener()
{
	Application& app = Lumen::getApp();

	// Setup socket.
	boost::asio::io_context ioContext{ 1 };
	// Assign to port 0 so that OS can supply an open port.
	tcp::acceptor acceptor{ ioContext, {*m_socketAddress.get(), 0}};
	m_port = acceptor.local_endpoint().port();

	std::string address = m_socketAddress->to_string();
	std::string portNum = std::to_string(m_port);
	std::string connectionMsg = "Connected to 'ws://" + address + ":" + portNum + "'.";
	app.pushNotification(NotificationType::Info, 5000, connectionMsg, "Websocket");
	LUMEN_LOG_INFO(connectionMsg, "Websocket");

	// Connection.
	tcp::socket webSocket{ ioContext };
	acceptor.accept(webSocket);
	LUMEN_LOG_INFO("Connection established.", "Websocket")
	// Handshake.
	basic_boost_websocket ws{ std::move(webSocket) };
	ws.accept();
	LUMEN_LOG_INFO("Handshake successful.", "Websocket");

	// Notify.
	app.pushNotification(NotificationType::Info, 5000, "Connection established.", "Websocket");

	// Read from socket.
	while (true)
	{
		boost::beast::flat_buffer buffer;
		ws.read(buffer);
		if (!buffer.size()) continue;
		std::string input = boost::beast::buffers_to_string(buffer.cdata());
		LUMEN_LOG_INFO("Received input.", "Websocket");

		// Dispatch script.
		if (input.find(LUA_EXECUTABLE_SCRIPT) != std::string::npos)
		{
			app.pushLuaScript(input);
		}
		else if (input.find(LUA_SCRIPT_GUI) != std::string::npos)
		{
			LumenWebSocket::createGuiFromScript(input);
		}
	}
}

void LumenWebSocket::createGuiFromScript(std::string& script) 
{
	static const std::string guiNameID = "-- Gui Name: '";
	static const std::string websocketID = "-- Websocket: '";

	Application& app = Lumen::getApp();

	// Find the gui name.
	std::string guiName;
	int pos = script.find(guiNameID.c_str());
	pos += guiNameID.size();
	while (script[pos] != '\'') 
	{
		guiName.push_back(script[pos++]);
	}

	ScriptGui* scriptGui = app.pushWindow<ScriptGui>(LumenDockPanel::Left, guiName);
	scriptGui->setSctipt(script);

	// Find the websocket url and connect GUI.
	pos = script.find(websocketID.c_str());
	pos += websocketID.size();
	// Get host.
	std::string host;
	while (script[pos] != ':')
	{
		host.push_back(script[pos++]);
	}
	// Get port.
	std::string port;
	pos++;
	while (script[pos] != '\'')
	{
		port.push_back(script[pos++]);
	}
	// Create connection.
	scriptGui->connectWebSocket(host, port);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//