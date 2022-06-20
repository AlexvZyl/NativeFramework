//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <iostream>
#include "Engines/EngineCore2D/EngineCore2D.h"
#include "OpenGL/SceneGL.h"
#include "Graphics/Camera/Camera.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "GUI/LumenGizmo/LumenGizmo.h"

//=============================================================================================================================================//
//  Costructor & Destructor.																												   //
//=============================================================================================================================================//

// With GLFW window.
EngineCore2D::EngineCore2D() 
	: EngineCore()
{
	// Scene setup.
	m_scene = std::make_unique<Scene>(CameraType::Standard2D, glm::vec2(500, 500));
	getScene().getCamera().setScaleRate(0.5f);

	// Bind the scene so that the engines can use the renderer on startup.
	// GraphicsScene unbinds restores and unbinds it when the engine contrsuction is complete.
	// This allows engines to use the renderer when they are not set as the active engine.
	// I do not think this is the best way of oding things...
	Renderer::storeAndBindScene(&getScene());

	// Gizmo setup.
	LumenGizmo& gizmo = getGizmo();
	gizmo.setCamera(getScene().getCamera());
	gizmo.setOperation(LumenGizmoOperation::Rotate2D);
	gizmo.setSnapValue(45.f);
};

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//