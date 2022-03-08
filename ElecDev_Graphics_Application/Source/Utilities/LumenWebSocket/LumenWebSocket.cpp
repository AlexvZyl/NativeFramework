//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include <thread>
#include "LumenWebSocket.h"
#include "External/Misc/ConsoleColor.h"
#include "Utilities/Platform/Thread.h"
#include "Lumen.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

using tcp = boost::asio::ip::tcp;

LumenWebSocket::LumenWebSocket(std::string& ip, unsigned short port)
	: m_port(port) 
{
	// Create connection and context.
	m_socketAddress = boost::asio::ip::make_address(ip);
	
	// Start listener thread.
	m_listenerThread = std::thread(LumenWebSocket::listener, this);

	// Log connection.
	std::cout << blue << "\n[LUMEN] [WEBSOCKET] : " << white << " Connected to '" << ip << ":" << std::to_string(port) << "'.\n";
}

LumenWebSocket::~LumenWebSocket()
{
	lumenTerminateThread(m_listenerThread);
}

void LumenWebSocket::listener(LumenWebSocket* socket) 
{
	// Setup socket.
	boost::asio::io_context ioContext{ 1 };
	tcp::acceptor acceptor{ ioContext, {socket->m_socketAddress, socket->m_port} };

	// Create connection.
	tcp::socket webSocket{ ioContext };
	acceptor.accept(webSocket);
	std::cout << blue << "\n[LUMEN] [WEBSOCKET] :" << white << " Connection established.\n";

	// Handshake.
	boost::beast::websocket::stream<tcp::socket> ws { std::move(webSocket) };
	ws.accept();
	std::cout << blue << "\n[LUMEN] [WEBSOCKET] :" << white << " Handshake successful.\n";

	// Read from socket.
	while (true) 
	{
		boost::beast::flat_buffer buffer;
		ws.read(buffer);
		std::string input = boost::beast::buffers_to_string(buffer.cdata());
		std::cout << input << "\n";
		if (input.find("Terminate") != std::string::npos )
		{
			Lumen::getApp().stopRunning();
		}
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//