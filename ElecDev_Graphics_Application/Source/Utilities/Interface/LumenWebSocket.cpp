//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include <thread>
#include "LumenWebSocket.h"
#include "External/Misc/ConsoleColor.h"
#include "Utilities/Platform/Thread.h"
#include "Application/Application.h"
#include "Utilities/Interface/ExternalInterface.h"

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

using tcp = boost::asio::ip::tcp;

LumenWebSocket::LumenWebSocket(const std::string& ip)
{
	// Create connection and context.
	m_socketAddress = boost::asio::ip::make_address(ip);
	
	// Start listener thread.
	m_listenerThread = std::thread(LumenWebSocket::listener, this);
}

LumenWebSocket::~LumenWebSocket()
{
	lumenTerminateThread(m_listenerThread);
}

void LumenWebSocket::listener(LumenWebSocket* socket) 
{
	// Setup socket.
	boost::asio::io_context ioContext{ 1 };
	// Assign to port 0 so that OS supplied an open port.
	tcp::acceptor acceptor{ ioContext, {socket->m_socketAddress, 0} };
	socket->m_port = acceptor.local_endpoint().port();

	// Log connection.
	std::cout << blue << "\n[LUMEN] [WEBSOCKET] : " << white << " Connected to '" << socket->m_socketAddress << ":" << socket->m_port << "'.";

	std::string address = socket->m_socketAddress.to_string();
	std::string portNum = std::to_string(socket->m_port);
	std::string connectionMsg = "Connected to 'ws://" + address + ":" + portNum + "'.";
	Lumen::getApp().pushNotification(NotificationType::Info, 5000, connectionMsg, "Websocket");

	// Create connection.
	tcp::socket webSocket{ ioContext };
	acceptor.accept(webSocket);
	std::cout << blue << "\n[LUMEN] [WEBSOCKET] :" << white << " Connection established.";

	// Handshake.
	boost::beast::websocket::stream<tcp::socket> ws { std::move(webSocket) };
	ws.accept();
	std::cout << blue << "[LUMEN] [WEBSOCKET] :" << white << " Handshake successful.\n";

	// Notify.
	Lumen::getApp().pushNotification(NotificationType::Info, 5000, "Connection established.", "Websocket");

	// Read from socket.
	while (true) 
	{
		boost::beast::flat_buffer buffer;
		ws.read(buffer);
		if (!buffer.size()) continue;
		std::string input = boost::beast::buffers_to_string(buffer.cdata());
		std::cout << input << "\n";
		ExternalInterface::parseInputString(input);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//