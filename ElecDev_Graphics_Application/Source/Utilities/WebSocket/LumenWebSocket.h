#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <memory>
#include <string>
#include <thread>

//==============================================================================================================================================//
//  Forward Declarations.																														//
//==============================================================================================================================================//

namespace boost {
namespace asio {
namespace ip {
	class address;
};};};

using boost_ip_address = boost::asio::ip::address;

//==============================================================================================================================================//
//  Web Socket Class.																															//
//==============================================================================================================================================//

class LumenWebSocket 
{
public:

	// Constructor.  Default to local host and get open port from OS.
	LumenWebSocket(const std::string& ip = "127.0.0.1");
	// Destructor.
	~LumenWebSocket();

	// Function that listens to web socket.
	void listener();

	// Data.
	std::unique_ptr<boost_ip_address> m_socketAddress = nullptr;
	int m_port = NULL;
	std::thread m_listenerThread;

private:

	// Websocket message types.
	inline static const char* LUA_SCRIPT_GUI = "LUA_SCRIPT_GUI";

	// Create a Lumen GUI.
	static void createGuiFromScript(std::string& script); 
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//