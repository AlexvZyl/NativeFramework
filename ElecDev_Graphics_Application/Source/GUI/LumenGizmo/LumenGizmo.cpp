#include "LumenGizmo.h"
#include "Utilities/Logger/Logger.h"
#include "Engines/CircuitDesigner/Peripherals/Component2D.h"
#include "Graphics/Camera/Camera.h"
#include "glm/gtc/matrix_transform.hpp"

float* LumenGizmo::computeReferenceMatrix()
{
	return &glm::translate(glm::mat4(1.f), glm::vec3(m_component->centre, 0.f) / m_camera->getTotalScale())[0][0];
}

float* LumenGizmo::computeViewMatrix() 
{
	return &glm::translate(glm::mat4(1.f), m_camera->getPosition() * m_camera->getTotalScale())[0][0];
}

void LumenGizmo::render()
{
	// Do not render if not enabled.
	if (!isEnabled() || !hasActiveEntity() || !isVisible()) return;

	// Setup the current gizmo.
	ImGuizmo::SetID(getID());
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetOrthographic(isOrthographic());
	ImGuizmo::SetRect(m_windowPosition.x, m_windowPosition.y, m_windowSize.x, m_windowSize.y);
	ImGuizmo::SetGizmoSizeClipSpace(m_size);

	// Update state.
	m_isUsing = ImGuizmo::IsUsing();
	m_isOver = ImGuizmo::IsOver();

	// Render the gizmo.
	Camera& camera = getCamera();
	glm::mat4 deltaMatrix(1.f);
	ImGuizmo::Manipulate(computeViewMatrix(), &camera.getProjectionMatrix()[0][0], getOperation(), getMode(), computeReferenceMatrix(), &deltaMatrix[0][0], &m_snapValue);

	// Use the delta matrix to manipulate the entity.
	float rotation[3], scaling[3], translation[3];
	ImGuizmo::DecomposeMatrixToComponents(&deltaMatrix[0][0], translation, rotation, scaling);
	if(rotation[2]) m_component->rotate(rotation[2]);
}