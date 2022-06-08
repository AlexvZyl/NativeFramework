#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/WebSocket/BoostWebsocket.h"
#include "Application/LumenWindow/LumenWindow.h"
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

namespace asio = boost::asio;
using asio_websocket = boost::asio::ip::tcp::socket;
using asio_ioc = asio::io_context;
using boost_error_code = boost::system::error_code;
using asio_endpoint = asio::ip::tcp::endpoint;

//==============================================================================================================================================//
//  Script GUI.																																	//
//==============================================================================================================================================//

class LumenWebsocketWindow : public LumenWindow
{
public:

	// Constructor.
	LumenWebsocketWindow(const std::string& name, int windowFlags = 0);
	// Destructor.
	virtual ~LumenWebsocketWindow();

	// Set the websocket the GUI callbacks to.
	void connectWebSocket(const std::string& host, const std::string& port);

	// Send a callback message over the web socket.
	void callbackMessage(const std::string& message); 

protected:

	// The websocket that the GUI callbacks to.
	std::unique_ptr<basic_boost_websocket> m_webSocket;
	std::unique_ptr<asio_ioc> m_ioContext;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
