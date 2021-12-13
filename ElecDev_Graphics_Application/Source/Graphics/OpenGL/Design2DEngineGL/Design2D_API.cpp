#include "Design2D_Engine.h"
#include "Peripherals/Component2D.h"
#include "Peripherals/Circuit.h"


void Design2DEngineGL::ComponentPlaceMode(glm::vec2 screenCoords) {
	if (designerState != COMPONENT_PLACE) {
		designerState = COMPONENT_PLACE;
		// Add a dummy component
		if (m_activeComponent) {
			m_activeComponent->unhighlight();
		}
		m_activeComponent = std::make_shared<Component2D>(screenCoords,
			m_triangleEntitiesVAO.get(),
			m_lineEntitiesVAO.get(),
			m_triangleTexturedEntitiesVAO.get(),
			m_circleEntitiesVAO.get(),
			m_circuit.get());
	}
}

void Design2DEngineGL::deleteComponent(std::shared_ptr<Component2D> component)
{
	if (component) {
		auto iterator = std::find(m_circuit->m_components.begin(), m_circuit->m_components.end(), m_activeComponent);
		if (iterator != m_circuit->m_components.end())
		{
			m_circuit->m_components.erase(iterator);
			m_activeComponent = NULL;
		}
	}
}

void Design2DEngineGL::deleteCable(std::shared_ptr<Cable> cable)
{
	if (cable) {
		auto iterator = std::find(m_circuit->m_cables.begin(), m_circuit->m_cables.end(), m_activeCable);
		if (iterator != m_circuit->m_cables.end())
		{
			m_circuit->m_cables.erase(iterator);
			m_activeCable = NULL;
		}
	}
}