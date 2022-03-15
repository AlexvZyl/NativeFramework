//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include <thread>
#include "LumenWebSocket.h"
#include "External/Misc/ConsoleColor.h"
#include "Utilities/Platform/Thread.h"
#include "Application/Application.h"
#include "Utilities/Lua/LuaInterpreter.h"
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

using tcp = boost::asio::ip::tcp;

LumenWebSocket::LumenWebSocket(const std::string& ip)
{
	// Create connection and context.
	m_socketAddress = boost::asio::ip::make_address(ip);

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
	// Assign to port 0 so that OS supplied an open port.
	tcp::acceptor acceptor{ ioContext, {m_socketAddress, 0} };
	m_port = acceptor.local_endpoint().port();

	std::string address = m_socketAddress.to_string();
	std::string portNum = std::to_string(m_port);
	std::string connectionMsg = "Connected to 'ws://" + address + ":" + portNum + "'.";
	app.pushNotification(NotificationType::Info, 5000, connectionMsg, "Websocket");
	LUMEN_LOG_INFO(connectionMsg, "WEBSOCKET");

	// Connection.
	tcp::socket webSocket{ ioContext };
	acceptor.accept(webSocket);
	LUMEN_LOG_INFO("Connection established.", "WEBSOCKET")
	// Handshake.
	boost::beast::websocket::stream<tcp::socket> ws{ std::move(webSocket) };
	ws.accept();
	LUMEN_LOG_INFO("Handshake successful.", "WEBSOCKET");

	// Notify.
	app.pushNotification(NotificationType::Info, 5000, "Connection established.", "Websocket");

	// Read from socket.
	while (true)
	{
		boost::beast::flat_buffer buffer;
		ws.read(buffer);
		if (!buffer.size()) continue;
		std::string input = boost::beast::buffers_to_string(buffer.cdata());
		LUMEN_LOG_INFO("Received input.", "WEBSOCKET");
		app.pushLuaScript(input);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//