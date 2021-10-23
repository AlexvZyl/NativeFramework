
// [NOTE]: glDebugMessageCallback() is also an option, but this method gives us better backwards compatibility for now.
// [GUIDE]:  Wrap each call to the OpenGL backend with glCall() and if an error occurs there will be more informatin
// on it in the terminal.

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "ErrorHandlerGL.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Clear the error flags.
void glClearErrors()
{
	// Clear the errors.
	while (glGetError() != GL_NO_ERROR);
}

// Check the error that occured.
bool  glCheckError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		// Print the error.
		std::cout	<< red << "\n\n[OPENGL] [ERROR] \n"
					<<		    "[HEX CODE]       : "	<< white << std::hex << error << std::dec << "\n"
					<< red <<   "[FILE]		 : "		<< white << file << "\n"
					<< red <<   "[LINE]		 : "		<< white << line << "\n"
					<< red <<   "[FUNCTION]	 : "		<< white << function << "\n";
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------