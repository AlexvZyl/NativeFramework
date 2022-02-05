/*
This is where the interactive 2D design engine is implemented.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <iostream>
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Entities/Primitive.h"
#include "OpenGL/Entities/Polygon.h"
#include "OpenGL/Entities/Circle.h"
#include "OpenGL/ShaderGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/CameraGL.h"
#include "OpenGL/RendererGL.h"
#include "Resources/ResourceHandler.h"
#include "External/Misc/ConsoleColor.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
Design2DEngine::Design2DEngine()
	: Base2DEngine()
{
	// Default circuit (In this case handled like a default scene).
	m_circuit = std::make_shared<Circuit>("Test", "AE");
	//m_guiState->design_engine = this;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//


