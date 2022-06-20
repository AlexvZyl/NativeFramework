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
	Renderer::storeAndBindScene(&getScene());
	getScene().getCamera().setScaleRate(0.5f);

	// Gizmo setup.
	LumenGizmo& gizmo = getGizmo();
	gizmo.setCamera(getScene().getCamera());
	gizmo.setOperation(LumenGizmoOperation::Rotate2D);
	gizmo.setSnapValue(45.f);
};

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//