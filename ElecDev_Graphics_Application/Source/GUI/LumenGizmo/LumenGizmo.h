#pragma once

#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"

class Component2D;
class Camera;

enum class LumenGizmoOperation
{
	Rotate2D = ImGuizmo::ROTATE_Z,
	Rotate3D = ImGuizmo::ROTATE,
	Translate2D = 1 | 2,				// ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Y
	Translate3D = ImGuizmo::TRANSLATE,
	Scale2D = 128 | 256,				// ImGuizmo::SCALE_X | ImGuizmo::SCALE_Y
	Scale3D = ImGuizmo::SCALE,
};

class LumenGizmo 
{
public:

	// Constructor.
	inline LumenGizmo() 
	{
		setMode(ImGuizmo::WORLD);
		setID(s_gizmoCount++);
	}

	// Mode & Operation.
	inline void setMode(ImGuizmo::MODE mode) { m_mode = mode; }
	inline ImGuizmo::MODE getMode() { return m_mode; }
	inline ImGuizmo::OPERATION getOperation() { return m_operation; }
	inline void enableSnap(float snapValue = 1.f) { m_snapValue = snapValue; }
	inline void setSnapValue(float snapValue) { m_snapValue = snapValue; }
	inline void disableSnap() { m_snapValue = 0.f; }
	inline bool isSnapEnabled() { return m_snapValue; };

	// Operation overloads.
	inline void setOperation(LumenGizmoOperation operation)
	{
		m_operation = (ImGuizmo::OPERATION) operation;
	}
	inline void addOperation(LumenGizmoOperation operation)
	{
		m_operation = m_operation | (ImGuizmo::OPERATION) operation;
	}

	// Projection type.
	inline void setOrthographic() { m_isOrthographic = true; }
	inline void setPerspective() { m_isOrthographic = false; }
	inline bool isOrthographic() { return m_isOrthographic; }
	inline bool isPerspective() { return !m_isOrthographic; }

	// Enabled & Using.
	inline bool isUsing() { return m_isUsing; }
	inline bool isOver() { return m_isOver; }
	inline bool isEnabled() { return m_enabled; }
	inline void enable() { m_enabled = true; }
	inline void disable() { m_enabled = false; }

	// Window.
	inline void setWindowSize(const glm::vec2& size) { m_windowSize = size; }
	inline void setWindowPosition(const glm::vec2& position) { m_windowPosition = position; }
	inline const glm::vec2& getWindowSize() { return m_windowSize; }
	inline const glm::vec2& getWindowPosition() { return m_windowPosition; }

	// Entities.
	inline void setEntity(Component2D* entity) { m_component = entity; }
	inline void clearEntities() { m_component = nullptr; }
	inline bool hasActiveEntity() { return m_component; }

	// Rendering.
	inline void setCamera(Camera& camera) { m_camera = &camera; }
	inline Camera& getCamera() { return *m_camera; }
	inline void setSize(float size) { m_size = size; }
	inline float getSize() { return m_size; };
	void render();

private:

	friend class EngineCore;
	inline void hide() { m_isHidden = true; }
	inline void visible() { m_isHidden = false; }
	inline bool isVisible() { return !m_isHidden; }

	// Transform to put the gizmo in the world.
	glm::mat4 computeReferenceMatrix();

	// ID.
	inline int getID() { return m_ID; }
	inline void setID(int ID) { m_ID = ID; }

	// Data.
	ImGuizmo::OPERATION m_operation;
	ImGuizmo::MODE m_mode;
	inline static int s_gizmoCount = 0;
	bool m_isOrthographic = true;
	glm::vec2 m_windowSize = {200.f, 200.f};
	glm::vec2 m_windowPosition = { 0.f, 0.f };
	int m_ID = 0;
	Camera* m_camera = nullptr;
	float m_size = 1.f;
	float m_snapValue = 1.f;
	glm::mat4 m_referenceMatrix;
	bool m_enabled = false;
	bool m_isOver = false;
	bool m_isUsing = false;
	bool m_isHidden = false;

	// Types of entities that can be edited.
	// This is highly inefficient atm.  We should really use model matrices...
	Component2D* m_component = nullptr;
};
