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

// ----------- //
//  D E B U G  //
// ----------- //

//#define LUMEN_LOG_DEBUG(contentStr)				\
//	LoggerMessage msg;							\
//	msg.content = contentStr;					\
//	msg.level = LoggerLevel::Debug;				\
//	Logger::pushMessage(msg);		

#define LUMEN_LOG_DEBUG(contentStr, titleStr)	\
	LoggerMessage msg;							\
	msg.content = contentStr;					\
	msg.title = titleStr;						\
	msg.level = LoggerLevel::Debug;				\
	Logger::pushMessage(msg);				

// --------- //
//  I N F O  //
// --------- //

/*#define LUMEN_LOG_INFO(contentStr)				\
	LoggerMessage msg;							\
	msg.content = contentStr;					\
	Logger::pushMessage(msg);	*/	

#define LUMEN_LOG_INFO(contentStr, titleStr)	\
	LoggerMessage msg;							\
	msg.content = contentStr;					\
	msg.title = titleStr;						\
	Logger::pushMessage(msg);				

// --------------- //
//  S U C C E S S  //
// --------------- //

//#define LUMEN_LOG_SUCCESS(contentStr)			\
//	LoggerMessage msg;							\
//	msg.content = contentStr;					\
//	msg.level = LoggerLevel::Success;			\
//	Logger::pushMessage(msg);		

#define LUMEN_LOG_SUCCESS(contentStr, titleStr)	\
	LoggerMessage msg;							\
	msg.content = contentStr;					\
	msg.title = titleStr;						\
	msg.level = LoggerLevel::Success;			\
	Logger::pushMessage(msg);			

// --------------- //
//  W A R N I N G  //
// --------------- //

//#define LUMEN_LOG_WARN(contentStr)				\
//	LoggerMessage msg;							\
//	msg.content = contentStr;					\
//	msg.level = LoggerLevel::Warning;			\
//	Logger::pushMessage(msg);		

#define LUMEN_LOG_WARN(contentStr, titleStr)	\
	LoggerMessage msg;							\
	msg.content = contentStr;					\
	msg.title = titleStr;						\
	msg.level = LoggerLevel::Warning;			\
	Logger::pushMessage(msg);			

// ----------- //
//  E R R O R  //
// ----------- //

//#define LUMEN_LOG_ERROR(contentStr)				\
//	LoggerMessage msg;							\
//	msg.content = contentStr;					\
//	msg.level = LoggerLevel::Error;				\
//	Logger::pushMessage(msg);		

#define LUMEN_LOG_ERROR(contentStr, titleStr)	\
	LoggerMessage msg;							\
	msg.content = contentStr;					\
	msg.title = titleStr;						\
	msg.level = LoggerLevel::Error;				\
	Logger::pushMessage(msg);	

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

class Logger 
{
public:

	// Push a message into the queue.
	static void pushMessage(LoggerMessage& msg);

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