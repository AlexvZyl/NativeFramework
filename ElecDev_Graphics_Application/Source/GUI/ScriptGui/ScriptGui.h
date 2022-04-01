#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/WebSocket/BoostWebsocket.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
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
//  Forward declerations.																														//
//==============================================================================================================================================//

struct lua_State;

//==============================================================================================================================================//
//  Script GUI.																																	//
//==============================================================================================================================================//

class ScriptGui : public GuiElementCore
{
public:

	// Constructor.
	ScriptGui(std::string name, int windowFlags);
	// Destructor.
	~ScriptGui();

	// Rendering.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

	// Set the websocket the GUI callbacks to.
	void connectWebSocket(std::string& host, std::string& port);

	// Set the script that defines the gui.
	void setSctipt(std::string& script);

	// Send a callback message over the web socket.
	void callbackMessage(std::string& message);

	// Tells the GUI to get a new script.
	void awaitNewScript();

	// The Lua Script that defines the GUI.
	std::string m_script;

private:

	// The websocket that the GUI callbacks to.
	std::unique_ptr<basic_boost_websocket> m_webSocket;
	std::unique_ptr<asio_ioc> m_ioContext;

	// Lua VM that the script is executed in.
	lua_State* m_luaState = nullptr;

	// Thread used for listening.
	std::thread m_listenerThread;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//