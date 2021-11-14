#pragma once

/*
The graphics engines are drawn in ImGUI by creating a window for each instance of every different window.
To be able to keep the engines dynamic and add different types as required they all come from the same parent,
the EngineCoreGL class.  This is the pointer type that will be used to point to all of the child classes and their
members.  However, this will require that the core contains all of the different functions that will be called, as well
as error handling for when a wrong function is called on a engine type.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Entities/Entity.h"	// For rendering entities in OpenGL.
#include "ErrorHandlerGL.h"		// For OpenGL error handling.
#include <glm.hpp>				// OpenGL maths.
#include "GUI/GUIState.h"		// The GUI states and other information.
#include <string>

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;
class FrameBufferObject;
class Shader;
class Vertex;
class VertexData;
class VertexDataTextured;
class VertexDataCircle;
class VertexDataTexturedCircle;
struct Font;

//=============================================================================================================================================//
//  Variables and constants.																												   //
//=============================================================================================================================================//

const float PI = 3.14159265358979323f;

//=============================================================================================================================================//
//  EngineCoreGL Class.																														   //
//=============================================================================================================================================//

class EngineCoreGL
{
public:

	// ----------------- //
	//  M A T R I C E S  //
	// ----------------- //

	// MVP Matrices.
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);					// The model matrix that places the object in the world.  Is going to be 
																// kept an identity matrix for now.
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);					// The matrix that handles the camera movement.
																// viewMatrix = translatinMatrix * rotationMatrix * scalingMatrix;
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);				// The matrix that handles the clipping plane (which part of the world is
																// going to be visible to the screen?)
	glm::mat4 m_viewportMatrix = glm::mat4(1.0f);				// The matrix that handles the viewport transform.  Converts screen pixel
																// coordinates to the OpenGL uniform coordinate system.

	// View matrix components.
	glm::mat4 m_scalingMatrix = glm::mat4(1.0f);				// Handles camera scaling.
	glm::mat4 m_translationMatrix = glm::mat4(1.0f);			// Handles camera translations.
	glm::mat4 m_rotationMatrix = glm::mat4(1.0f);				// Handles camera rotations.

	// We need matrices to store the base view of the drawing.  This is to fall back to when right clicking, and this has to 
	// be updated with resizing and when auto sizing and scaling funtions are called.
	glm::mat4 m_scalingMatrixBase = glm::mat4(1.0f);			// Stores base matrix for camera scaling.
	glm::mat4 m_translationMatrixBase = glm::mat4(1.0f);		// Stores base matrix for camera translation.
	glm::mat4 m_rotationMatrixBase = glm::mat4(1.0f);			// Stores base matrix for camera rotation.

	// ---------------------------------- //
	//  R E N D E R I N G   O B J E C T S //
	// ---------------------------------- //

	// Shaders.
	std::unique_ptr<Shader> m_basicShader;						// Renders movable elements without textures.
	std::unique_ptr<Shader> m_textureShader;					// Renders movable elements with textures.
	std::unique_ptr<Shader> m_backgroundShader;					// Renders the background.
	std::unique_ptr<Shader> m_circleShader;						// Renders circles.

	// Vertex arrays.
	std::unique_ptr<VertexArrayObject<VertexData>> m_linesVAO;								// Lines.
	std::unique_ptr<VertexArrayObject<VertexData>> m_trianglesVAO;							// Triangles.
	std::unique_ptr<VertexArrayObject<VertexDataTextured>> m_texturedTrianglesVAO;			// Textured Triangles.
	std::unique_ptr<VertexArrayObject<VertexDataCircle>> m_circlesVAO;						// Normal circles.
	std::unique_ptr<VertexArrayObject<VertexData>> m_lineEntitiesVAO;						// Drawing line entities.
	std::unique_ptr<VertexArrayObject<VertexData>> m_triangleEntitiesVAO;					// Drawing triangle entities.
	std::unique_ptr<VertexArrayObject<VertexDataTextured>> m_triangleTexturedEntitiesVAO;	// Drawing textured triangle entities.
	std::unique_ptr<VertexArrayObject<VertexDataCircle>> m_circleEntitiesVAO;				// Drawing circle entities.
	std::unique_ptr<VertexArrayObject<VertexData>> m_backgroundVAO;							// Background has a seperate VAO since it should not move.

	// Frame Buffer Object.
	std::unique_ptr<FrameBufferObject> m_frameBuffer;			// FBO to render scene onto.  Stores the OpenGL scene as a texture.
																// Also implements MSAA.

	std::unique_ptr<Font> m_defaultFont;						// The default font for the rendering engine.

	std::vector<Entity<VertexDataTextured>> m_textEnities;		// Vector containing the entties to be rendered.

	// --------------------------- //
	//  S T A T E   M A C H I N E  //
	// --------------------------- //

	GUIState* m_guiState;										// Contains the current state of the application as well as information
																// that is required from the GUI side for the Graphics side.

	// ----------------- //
	//  V I E W P O R T  //
	// ----------------- //

	float m_imGuiViewportDimensions[2] = { 500, 500 };			// Stores the dimensions of the viewport that the OpenGL context gets drawn to.  

	// Resizes the viewport, projection matrix and FBO.
	virtual void resizeEvent(float width, float height);

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	EngineCoreGL(GUIState* guiState);
	~EngineCoreGL();

	// --------------------------------------- //
	//  R E N D E R I N G   F U N C T I O N S  //
	// --------------------------------------- //

	// Main loop where the rendering happens.
	virtual void renderLoop();
	// Updates the buffers on the GPU with the data stored CPU side.  Required 
	// to call to update the elements that are displayed to the screen.
	virtual void updateGPU();
	// Autocenters the current scene.
	virtual void autoCenter();
	// Draws elements to the screen.  Used for debugging and benchmarking.
	// Should also be implemented to showcase the capabilities of the engine.
	virtual void drawDemo(unsigned int loopCount);
	// Returns the FBO texture ID that can be rendered.
	virtual unsigned int getRenderTexture();
	// Creates the default background for the engines.
	void createDefaultBackground();
	// Calculates the delta time and returns it.	
	// This is used to ensure constant movement and sensitivity
	// regardless of the framerate.
	float deltaTime();
	// Returns the ID of the entity in the pixel coords.
	unsigned int getEntityID(float pixelCoords[2]);
	// Calculate the world coordinates from the pixel coordinates.
	virtual glm::vec3 pixelCoordsToWorldCoords(float pixelCoords[2]);
	// Calculate the camera coordinates from the pixel coordinates.
	virtual glm::vec3 pixelCoordsToCameraCoords(float pixelCoords[2]);

	float m_deltaTime = 0.0f;	// The difference between the last 2 rendred frame.
	float m_lastFrame = 0.0f;	// The time of the previous frame.

	// --------------------- //
	//  U S E R   I N P U T  //
	// --------------------- //

	float m_prevMouseEventPixelCoords[2] = { NULL, NULL };		// Stores the previous mouse event information.
	glm::vec3 m_prevMouseEventWorldVec = { NULL, NULL, NULL };	// Stores the vector that goes into the world.
	virtual void mousePressLeft(float pixelCoords[2]);
	virtual void mousePressRight(float pixelCoords[2]);
	virtual void mousePressMiddle(float pixelCoords[2]);
	virtual void mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle);
	virtual void mouseScrollEvent(float pixelCoords[2], float yOffset);
	virtual void keyEvent(int key, int action);

	// ------------- //
	//  2 D   A P I  //
	// ------------- //

	virtual void drawLine(float position1[2], float position2[2], float color[4]);
	virtual void drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4]);
	virtual void drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4]);
	virtual void drawQuadClear(float position[2], float width, float height, float color[4]);
	virtual void drawQuadFilled(float position[2], float width, float height, float color[4]);
	virtual void drawCircleClear(float position[2], float radius, float color[4]);
	virtual void drawCircleFilled(float position[2], float radius, float color[4]);
	virtual void drawText(std::string text, float coords[2], float color[4], float scale, std::string align);

	// ------------- //
	//  3 D   A P I  //
	// ------------- //

	virtual void drawQuadFilled3D(float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float color[4]);
	virtual void drawCuboidFilled(float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float depth, float color[4]);

	// ------------------------------- //
	//  E R R O R   F U N C T I O N S  //
	// ------------------------------- //

	void functionNotImplementedError(std::string functionName);	// This is called when a funtion in EngineCoreGL is called but was not 
																// implemented in the child engine.
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//