#pragma once

// [NOTE]: glDebugMessageCallback() is also an option, but this method gives us better backwards compatibility for now.

// [GUIDE]:  Wrap each call to the OpenGL backend with glCall() and if an error occurs there will be more informatin
// on it in the terminal.

//----------------------------------------------------------------------------------------------------------------------
//  Compiler settings.
//----------------------------------------------------------------------------------------------------------------------

// Breakpoint at OpenGL error.
#define	ASSERT(x) if (!(x)) __debugbreak();
// Create macro for each OpenGL call.  Only runs if 
#ifdef	_DEBUG
#define glCall(x)	glClearErrors();\
					x;\
					ASSERT(glCheckError(#x, __FILE__, __LINE__));
#else
glCall(x)	x;
#endif // _DEBUG

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Clear the error flags.
void glClearErrors();

// Check the error that occured.
bool  glCheckError(const char* function, const char* file, const char* line);

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------