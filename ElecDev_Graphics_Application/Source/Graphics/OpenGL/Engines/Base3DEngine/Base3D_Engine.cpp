//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// The base 2D engine.
#include "Base3D_Engine.h"

//=============================================================================================================================================//
//  Constructor and Destructor.																												   //
//=============================================================================================================================================//

// Constructor.  Init with BaseEngineGL constructor.
Base3DEngineGL::Base3DEngineGL(stateMachine* states) : BaseEngineGL(states)
{
	// Starting log.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 3D engine starting...";

	// Create the camera.
	float position[3] = {0.0f, 0.0f, 1.0f};
	float target[3] = { 0.0f, 0.0f, 0.0f };
	m_camera = new Camera(position, target, &m_rotationMatrix, &m_translationMatrix);

	// Create the background (or skybox) for the 3D scene.
	createBackground();

	// The projection matrix has to be recalculated for a 3D projection, using a perspective projection.
	m_projectionMatrix = glm::perspective(glm::radians(45.0f), m_imGuiViewportDimensions[0]/m_imGuiViewportDimensions[1], 0.1f, 100.0f);

	// Done log.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 3D engine done.";
}

// Destructor.
Base3DEngineGL::~Base3DEngineGL() 
{
	// Clear 3D engine memory.
	delete m_camera;

	// Call parent destructor.
	BaseEngineGL::~BaseEngineGL();
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

// [MAIN LOOP] Rendering loop.
void Base3DEngineGL::renderLoop() 
{
	// Enable depth test for the 3D rendering.
	GLCall(glEnable(GL_DEPTH_TEST));

	// Call parent render loop.
	BaseEngineGL::renderLoop();

	// Disable depth test since its not needed for the 2D engines.
	GLCall(glDisable(GL_DEPTH_TEST));
}

// Applies the render loop matrix calculations.
// This function is called in 'BaseEngineGL::renderLoop()'.
void Base3DEngineGL::updateMatrices()
{
	// Update the rotation matrix from the quaternion.
	m_camera->updateMatrices();

	// Call base function.
	BaseEngineGL::updateMartrices();
}

// Creates the background for the scene.
void Base3DEngineGL::createBackground() 
{
	// Delete the Base2D background.
	delete m_backgroundVAO;

	// Now create the background for the 3D scene.
	m_backgroundVAO = new VertexArrayObject(GL_TRIANGLES);
	// Assign background data.
	float bgColor1[4] = { (float)162 / 255, (float)184 / 255, (float)242 / 255, 1.0f };
	float bgColor2[4] = { (float)210 / 255, (float)242 / 255, (float)255 / 255, 1.0f };
	VertexData v5(1.0f, 1.0f, 0.99f, bgColor2[0], bgColor2[1], bgColor2[2], bgColor2[3]);	//  Top right.
	VertexData v6(-1.0f, 1.0f, 0.99f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3]);	//  Top left.
	VertexData v7(-1.0f, -1.0f, 0.99f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3]);	//  Bottom left.
	VertexData v8(1.0f, -1.0f, 0.99f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3]);	//  Bottom right.
	std::vector<VertexData> vertices = { v5, v6, v7, v7, v8, v5 };
	// Create background.
	m_backgroundVAO->writeData(vertices);
	m_backgroundVAO->updateGPU();
}


//=============================================================================================================================================//
//  Window events.																															   //
//=============================================================================================================================================//

void Base3DEngineGL::resizeEvent(int width, int height) 
{
	// Set new viewport values.
	m_imGuiViewportDimensions[0] = (float)width;
	m_imGuiViewportDimensions[1] = (float)height;

	// Now recalculate the projection matrix.
	m_projectionMatrix = glm::perspective(glm::radians(45.0f), m_imGuiViewportDimensions[0] / m_imGuiViewportDimensions[1], 0.1f, 100.0f);

	// Apply changes to shaders.
	m_basicShader->bind();
	m_basicShader->setMat4("projectionMatrix", m_projectionMatrix);
	m_textureShader->bind();
	m_textureShader->setMat4("projectionMatrix", m_projectionMatrix);
	
	// Resize the frame buffer.
	m_frameBuffer->resize(width, height);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//