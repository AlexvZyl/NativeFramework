#include "LumenGizmo.h"
#include "Utilities/Logger/Logger.h"
#include "Engines/CircuitDesigner/Peripherals/Component2D.h"
#include "Graphics/Camera/Camera.h"
#include "glm/gtc/matrix_transform.hpp"

glm::mat4 LumenGizmo::computeReferenceMatrix()
{
	glm::mat4 referenceMatrix = glm::mat4(1.f);
	referenceMatrix = glm::translate(referenceMatrix, glm::vec3(m_component->centre, 0.f));
	//referenceMatrix = glm::scale(referenceMatrix,  m_camera->getTotalScale());
	return referenceMatrix;
}

const float* LumenGizmo::computeViewMatrix() 
{
	return &m_camera->getViewMatrix()[0][0];
}

void LumenGizmo::render()
{
	// Set current gizmo.
	ImGuizmo::SetID(getID());

	// Update state.
	m_isUsing = ImGuizmo::IsUsing();
	m_isOver = ImGuizmo::IsOver();

	// Do not render if not enabled.
	if (!isEnabled() || !hasActiveEntity() || !isVisible()) return;

	// Set current gizmo data.
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetOrthographic(isOrthographic());
	ImGuizmo::SetRect(m_windowPosition.x, m_windowPosition.y, m_windowSize.x, m_windowSize.y);
	ImGuizmo::SetGizmoSizeClipSpace(m_size / m_camera->getTotalScale().x);

	// Render the gizmo.
	Camera& camera = getCamera();
	glm::mat4 deltaMatrix(1.f);
	ImGuizmo::Manipulate(computeViewMatrix(), &camera.getProjectionMatrix()[0][0], getOperation(), getMode(), &computeReferenceMatrix()[0][0], &deltaMatrix[0][0], &m_snapValue);

	// Use the delta matrix to manipulate the entity.
	float rotation[3], scaling[3], translation[3];
	ImGuizmo::DecomposeMatrixToComponents(&deltaMatrix[0][0], translation, rotation, scaling);
	if(rotation[2]) m_component->rotate(rotation[2]);
}