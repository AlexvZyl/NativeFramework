
class VertexBufferObject 
{
public:

	// Constructor should do nothing.
	inline VertexBufferObject() = default;

	// Destructor.
	~VertexBufferObject();

	// Create the buffer.
	void createBuffer();

	// Delete the buffer.
	void deleteBuffer();

	// Check if the buffer exists on the GPU.
	bool existsOnGPU();

	// Get the VBO ID.
	int ID();
		
	// Bind the VBO.
	void bind();

	// Unbind the VBO.
	void unbind();

	// Set the data.
	void bufferData(int size, const void* data, int usage);

	// Set the data by name.
	void namedBufferData(int size, const void* data, int usage);

	// Set the buffer sub data.
	void bufferSubData(int offset, int size, const void* data);

	// Set the buffer sub data by name.
	void namedBufferSubData(int offset, int size, const void* data);

	// Get the capacity.
	int capacity();

private:

	unsigned m_rendererID = NULL;
	int m_capacity = NULL;
	bool m_existsOnGPU = false;
};