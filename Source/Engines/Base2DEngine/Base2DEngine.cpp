//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <iostream>
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "OpenGL/SceneGL.h"
#include "Graphics/Camera/Camera.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "GUI/LumenGizmo/LumenGizmo.h"

//=============================================================================================================================================//
//  Costructor & Destructor.																												   //
//=============================================================================================================================================//

// With GLFW window.
Base2DEngine::Base2DEngine() 
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