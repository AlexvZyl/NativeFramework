#include "ComponentDesigner.h"


ComponentDesigner::ComponentDesigner():Base2DEngine()
{
	m_Component = std::make_shared<Component2D>(Component2D(nullptr));
}
