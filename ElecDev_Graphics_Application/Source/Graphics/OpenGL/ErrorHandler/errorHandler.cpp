
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
#include <ErrorHandler/errorHandler.h>

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Clear the error flags.
void glClearErrors()
{
	// Run the loop while there is an error.
	while (!glGetError());

}

// Check the error that occured.
bool  glCheckError(const char* function, const char* file, const char* line)
{
	while (GLenum error = glGetError())
	{
		// Print the error.
		std::cout << "\n[OPENGL ERROR (HEX)] : {" << std::hex << error << std::dec << "}\n" <<
			"[FUNCTION] : {" << function << "}\n" <<
			"[FILE] : {" << file << "}\n" <<
			"[LINE] : {" << line << "}\n";
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------