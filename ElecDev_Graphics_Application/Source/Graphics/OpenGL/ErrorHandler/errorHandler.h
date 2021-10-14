#pragma once

// [NOTE]: glDebugMessageCallback() is also an option, but this method gives us better backwards compatibility for now.

// [GUIDE]:  Wrap each call to the OpenGL backend with glCall() and if an error occurs there will be more informatin
// on it in the terminal.

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// General.
#include <iostream>

// OpenGL.
#include <glad/glad.h>

// Console coloring.
#include <External/Misc/ConsoleColor.h>

//----------------------------------------------------------------------------------------------------------------------
//  Compiler settings.
//----------------------------------------------------------------------------------------------------------------------

// Breakpoint at OpenGL error.
#define	ASSERT(x) if (!(x)) __debugbreak();

// Create macro for each OpenGL call.  Only runs if in debug mode.
#ifdef	_DEBUG
	#define GLCall(x)	glClearErrors();\
						x;\
						ASSERT(glCheckError(#x, __FILE__, __LINE__));
#else
	#define GLCall(x)	x;
#endif // _DEBUG

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Clear the error flags.
void glClearErrors();

// Check the error that occured.
bool  glCheckError(const char* function, const char* file, int line);

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------