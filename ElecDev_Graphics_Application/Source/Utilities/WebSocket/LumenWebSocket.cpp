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

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

using tcp = boost::asio::ip::tcp;

LumenWebSocket::LumenWebSocket(const std::string& ip)
{
	Application& app = Lumen::getApp();

	// Create connection and context.
	m_socketAddress = boost::asio::ip::make_address(ip);
	
	// Setup socket.
	boost::asio::io_context ioContext{ 1 };
	// Assign to port 0 so that OS supplied an open port.
	tcp::acceptor acceptor{ ioContext, {m_socketAddress, 0} };
	m_port = acceptor.local_endpoint().port();

	std::string address = m_socketAddress.to_string();
	std::string portNum = std::to_string(m_port);
	std::string connectionMsg = "Connected to 'ws://" + address + ":" + portNum + "'.";
	app.pushNotification(NotificationType::Info, 5000, connectionMsg, "Websocket");

	// Create connection.
	tcp::socket webSocket{ ioContext };
	acceptor.accept(webSocket);
	// Log connection.
	m_webSocket = std::make_unique<boost::beast::websocket::stream<tcp::socket>>( std::move(webSocket) );
	m_webSocket->accept();
	// Log handshake.

	// Notify of connection.
	app.pushNotification(NotificationType::Info, 5000, "Connection established.", "Websocket");
	// Log connection in PIPE.
	std::cout << blue << "[LUMEN] [WEBSOCKET] : " << white << " Connected to '" << m_socketAddress << ":" << m_port << "'.";
	std::cout.flush();

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

	// Read from socket.
	while (true) 
	{
		boost::beast::flat_buffer buffer;
		m_webSocket->read(buffer);
		if (!buffer.size()) continue;
		std::string input = boost::beast::buffers_to_string(buffer.cdata());
		app.pushLuaScript(input);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//