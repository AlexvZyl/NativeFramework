#include "ComponentDesigner.h"


ComponentDesigner::ComponentDesigner():Base2DEngine()
{
	m_activeComponent = std::make_shared<Component2D>(nullptr);
	m_activeComponent->place(glm::vec2(0.f));
}
