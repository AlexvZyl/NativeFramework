#include "Design2D_Engine.h"
#include "Peripherals/Component2D.h"


void Design2DEngineGL::ComponentPlaceMode(float screenCoords[2]) {
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