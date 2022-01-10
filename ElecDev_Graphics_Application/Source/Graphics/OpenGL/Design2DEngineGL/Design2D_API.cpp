#include "Design2D_Engine.h"
#include "Peripherals/Component2D.h"
#include "Peripherals/Circuit.h"
#include "CoreGL/RendererGL.h"

void Design2DEngineGL::ComponentPlaceMode(glm::vec2 screenCoords) 
{
	if (designerState != COMPONENT_PLACE) 
	{
		designerState = COMPONENT_PLACE;
		// Add a dummy component
		if (m_activeComponent) 
		{
			m_activeComponent->unhighlight();
		}
		Renderer::bindScene(m_scene.get());
		m_activeComponent = std::make_shared<Component2D>(screenCoords, m_circuit.get());
	}
}

void Design2DEngineGL::deleteActiveComponent()
{
	if (m_activeComponent) 
	{
		auto iterator = std::find(m_circuit->m_components.begin(), m_circuit->m_components.end(), m_activeComponent);
		if (iterator != m_circuit->m_components.end())
		{
			Renderer::bindScene(m_scene.get());
			m_circuit->m_components.erase(iterator);
			m_activeComponent = nullptr;
			m_guiState->active_component = nullptr;
		}
	}
}

void Design2DEngineGL::deleteActiveCable()
{
	if (m_activeCable) 
	{
		auto iterator = std::find(m_circuit->m_cables.begin(), m_circuit->m_cables.end(), m_activeCable);
		if (iterator != m_circuit->m_cables.end())
		{
			Renderer::bindScene(m_scene.get());
			m_circuit->m_cables.erase(iterator);
			m_activeCable = nullptr;
		}
	}
}