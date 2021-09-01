#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <ErrorHandler/errorHandler.h>

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Buffer Class.
//----------------------------------------------------------------------------------------------------------------------

class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int count;

public:

	// Constructor.
	IndexBuffer(const unsigned int* data, unsigned int count);
	// Destructor.
	~IndexBuffer();

	// Default constructor.
	IndexBuffer();

	// Functions.
	void bind() const;
	void unbind() const;
	
	// Get count value.
	inline unsigned int GetCount() const { return count; }
};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------