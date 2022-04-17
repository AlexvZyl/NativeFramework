#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include <vector>

//=============================================================================================================================================//
//  Data.																																	   //
//=============================================================================================================================================//

enum class LoggerLevel 
{
	Debug,
	Info,
	Success,
	Warning,
	Error
};

struct LoggerMessage 
{
	std::string content = "";
	std::string title = "";
	LoggerLevel level = LoggerLevel::Info;
};

//=============================================================================================================================================//
//  Macros.																																	   //
//=============================================================================================================================================//

//  NOTE: The title string can be an empty string, the default title will then be used.
//		  Errors are also pushed as notifications.

// ----------- //
//  D E B U G  //
// ----------- //	

#ifdef _DEBUG										
#define LUMEN_LOG_DEBUG(contentStr, titleStr)	\
{												\
	LoggerMessage loggerMessage;				\
	loggerMessage.content = contentStr;			\
	loggerMessage.title = titleStr;				\
	loggerMessage.level = LoggerLevel::Debug;	\
	Logger::pushMessage(loggerMessage);			\
}		
#else
#define LUMEN_LOG_DEBUG(contentStr, titleStr)	
#endif

// --------- //
//  I N F O  //
// --------- //

#define LUMEN_LOG_INFO(contentStr, titleStr)	\
{												\
	LoggerMessage loggerMessage;				\
	loggerMessage.content = contentStr;			\
	loggerMessage.title = titleStr;				\
	loggerMessage.level = LoggerLevel::Info;	\
	Logger::pushMessage(loggerMessage);			\
}

// --------------- //
//  S U C C E S S  //
// --------------- //

#define LUMEN_LOG_SUCCESS(contentStr, titleStr)	\
{												\
	LoggerMessage loggerMessage;				\
	loggerMessage.content = contentStr;			\
	loggerMessage.title = titleStr;				\
	loggerMessage.level = LoggerLevel::Success;	\
	Logger::pushMessage(loggerMessage);			\
}

// --------------- //
//  W A R N I N G  //
// --------------- //	

#define LUMEN_LOG_WARN(contentStr, titleStr)	\
{												\
	LoggerMessage loggerMessage;				\
	loggerMessage.content = contentStr;			\
	loggerMessage.title = titleStr;				\
	loggerMessage.level = LoggerLevel::Warning;	\
	Logger::pushMessage(loggerMessage);			\
}

// ----------- //
//  E R R O R  //
// ----------- //	

#define LUMEN_LOG_ERROR(contentStr, titleStr)	\
{												\
	LoggerMessage loggerMessage;				\
	loggerMessage.content = contentStr;			\
	loggerMessage.title = titleStr;				\
	loggerMessage.level = LoggerLevel::Error;	\
	Logger::pushMessage(loggerMessage);			\
}

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

class Logger 
{
public:

	// Push a message into the queue.
	static void pushMessage(const LoggerMessage& msg);

	// Log all of the messages in the queue.
	static void flushQueue();

	// Clear all of the messgages in the queue.
	static void clear();

private:

	// Private constructor.
	inline Logger() = default;

	// Queue containing messages.
	inline static std::vector<LoggerMessage> s_messageQueue;

};

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//