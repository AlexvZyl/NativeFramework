
//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Application/ApplicationTemplates.h"
#include "CircuitDesigner.h"
#include "Peripherals/Component2D.h"
#include "Peripherals/Circuit.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "Graphics/Entities/EntityManager.h"
#include "Application/Application.h"
#include "GUI/CircuitDesignerPopupModal.h"
#include "GUI/LumenGizmo/LumenGizmo.h"
#include "glm/gtc/matrix_transform.hpp"

// Utility for comparing two vecs.
bool compare(const glm::vec2& vec1, const glm::vec2& vec2, float tolerance = 1e-6);

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

void CircuitDesigner::ComponentPlaceMode(glm::vec2 screenCoords)
{
	if (designerState != COMPONENT_PLACE)
	{
		designerState = COMPONENT_PLACE;
		// Add a dummy component.
		if (m_activeComponent) m_activeComponent->disableOutline();

		m_activeComponent = std::make_shared<Component2D>(screenCoords, m_circuit.get());
	}
}

void CircuitDesigner::ComponentPlaceMode()
{
	glm::vec2 pos = getMouseLocalPosition();
	pos = pixelToWorldCoords(pos); 
	ComponentPlaceMode(pos);
}

void CircuitDesigner::deleteActiveComponent()
{
	if (!m_activeComponent) return;
	deleteComponent(m_activeComponent.get());
}

void CircuitDesigner::deleteActiveCable()
{
	if (!m_activeCable) return;

	auto iterator = std::find(m_circuit->m_cables.begin(), m_circuit->m_cables.end(), m_activeCable);
	if (iterator != m_circuit->m_cables.end())
	{
		m_circuit->m_cables.erase(iterator);
		m_activeCable = nullptr;
	}
}

void CircuitDesigner::setActiveComponent(unsigned eID) 
{
	if (m_activeComponent)
	{
		m_activeComponent->disableOutline();
		m_activeComponent = NULL;
	}
	if ((eID == 0) || (eID == -1))
	{
	}
	else
	{
		Entity* currentEntity = EntityManager::getEntity(eID);
		if (!currentEntity) return;
		currentEntity->setContext();
		while (currentEntity->m_type != EntityType::COMPONENT)
		{
			currentEntity = currentEntity->m_parent;
			// Nothing was clicked.
			if (!currentEntity) return;
			if (currentEntity->m_parent == nullptr) return;
		}

		// This cast remains valid provided all entities on screen are decendents of components. 
		// If not, this needs to change.
		Component2D* cur = dynamic_cast<Component2D*>(currentEntity);
		m_activeComponent = *std::find_if(begin(m_circuit->m_components), end(m_circuit->m_components), [&](std::shared_ptr<Component2D> current)
			{
				return current.get() == cur;
			});
		m_activeComponent->enableOutline();
	}
}
void CircuitDesigner::setActiveCable(unsigned eID) 
{
	if (m_activeCable)
	{
		m_activeCable->disableOutline();
		m_activeCable = NULL;
	}
	if ((eID == 0) || (eID == -1)) 
	{
	}
	else 
	{
		Entity* currentEntity = EntityManager::getEntity(eID);

		if (!currentEntity) return;

		currentEntity->setContext();
		while (currentEntity->m_parent != nullptr) 
		{
			if (currentEntity->m_parent->m_type == EntityType::CABLE) 
			{
				// User clicked on a component. Set this cable as active, and send the primative to the component.
				Cable* cur = dynamic_cast<Cable*>(currentEntity->m_parent);
				//m_activeComponent = dynamic_cast<std::shared_ptr>(cur);
				m_activeCable = *std::find_if(begin(m_circuit->m_cables), end(m_circuit->m_cables), [&](std::shared_ptr<Cable> current)
					{
						return current.get() == cur;
					});
				m_activeCable->enableOutline();
			}
			currentEntity = currentEntity->m_parent;
		}
	}
}

Port* CircuitDesigner::getPort(unsigned eID)
{
	if ((eID == 0) || (eID == -1))
	{
		return nullptr;
	}
	else
	{
		Entity* currentEntity = EntityManager::getEntity(eID);
		if (!currentEntity) return nullptr;
		while (currentEntity->m_type != EntityType::PORT)
		{
			currentEntity = currentEntity->m_parent;
			if (!currentEntity) return nullptr;
		}

		// This cast remains valid provided all entities on screen are decendents of components. 
		// If not, this needs to change.
		Port* cur = dynamic_cast<Port*>(currentEntity);
		return cur;
	}
}

void CircuitDesigner::setNameOfElements(const std::string& name)
{
	m_circuit->m_label = name;
}

void CircuitDesigner::loadAndPlaceComponent(const std::filesystem::path& path, const glm::vec2& mousePos) 
{
	if (m_activeComponent)
		m_activeComponent->disableOutline();
	m_circuit->m_components.push_back(std::make_shared<Component2D>(path, m_circuit.get()));
	m_circuit->m_components.back()->move(getNearestGridVertex(pixelToWorldCoords(mousePos)));
	m_activeComponent = m_circuit->m_components.back();
	designerState = ENTITY_SELECT;
	getGizmo().setEntity(m_activeComponent.get());
}

void CircuitDesigner::loadAndPlaceComponent(const YAML::Node& node, const glm::vec2& mousePos)
{
	if(m_activeComponent)
		m_activeComponent->disableOutline();
	m_circuit->m_components.push_back(std::make_shared<Component2D>(node, m_circuit.get()));
	m_circuit->m_components.back()->move(getNearestGridVertex(pixelToWorldCoords(mousePos)));
	m_activeComponent = m_circuit->m_components.back();
	designerState = ENTITY_SELECT;
	getGizmo().setEntity(m_activeComponent.get());
}

void CircuitDesigner::loadDataToCable(const YAML::Node& node, Cable* cable)
{
	if (!cable) return;

	YAML::Node cableNode = node;
	if (node["Cable"].IsDefined()) cableNode = node["Cable"];
	// Not ideal... FIX.
	else if (cableNode["Ports"].IsDefined() || cableNode["Component"].IsDefined()) return;

	// Load type.
	cable->m_cableType = cableNode["Label"].as<std::string>();
	// Load color.
	cable->setColour({
		cableNode["Color"][0].as<float>(),
		cableNode["Color"][1].as<float>(),
		cableNode["Color"][2].as<float>() ,
		cableNode["Color"][3].as<float>()
	});
	// Load dictionary.
	cable->cableDict.clear();
	for (const auto& keyValPair : cableNode["Dictionary"])
		cable->cableDict.insert({ keyValPair.first.as<std::string>(), keyValPair.second.as<std::string>() });
}

bool operator==(std::shared_ptr<Component2D> compSP, Component2D* compRP) 
{
	return compSP.get() == compRP;
}

bool operator==(std::shared_ptr<Cable> cableSP, Cable* cableRP)
{
	return cableSP.get() == cableRP;
}

void CircuitDesigner::importComponent(const std::filesystem::path& path, bool loadOnImport, bool checkForOverwrite)
{
	// Check if it is a component file.
	if (path.filename().extension().string() != ".lmcp")
	{
		LUMEN_LOG_WARN("Tried to import component with invalid extension.", "");
		return;
	}

	// Add component if it does not exist.
	std::string compName = path.filename().string();
	if (!checkForOverwrite || m_circuit->m_referenceComponents.find(compName) == m_circuit->m_referenceComponents.end())
	{
		m_circuit->m_referenceComponents.insert({ compName, YAML::LoadFile(path.string())["Component"] });
		if (loadOnImport)  loadAndPlaceComponent(path, getMouseLocalPosition());
	}
	// Warn user of component overwrite.
	else
	{
		auto* modal = Lumen::getApp().pushWindow<CircuitDesignerPopupModal>(LumenDockPanel::Floating, "Component Overwrite");
		modal->m_entityPath = path;
		modal->m_componentOverwrite = true;
		if (loadOnImport)
			modal->m_mousePosition = getMouseLocalPosition();
	}
}

void CircuitDesigner::importCable(const std::filesystem::path& path, bool loadOnImport, bool checkForOverwrite)
{
	// Check if it is a component file.
	if (path.filename().extension().string() != ".lmcb")
	{
		LUMEN_LOG_WARN("Tried to import cable with invalid extension.", "");
		return;
	}

	// Add cable if it does not exist.
	std::string cableName = path.filename().string();
	if (!checkForOverwrite || m_circuit->m_referenceCables.find(cableName) == m_circuit->m_referenceCables.end())
	{
		m_circuit->m_referenceCables.insert({ cableName, YAML::LoadFile(path.string())["Cable"] });
		if(loadOnImport) loadDataToCable(YAML::LoadFile(path.string()), m_activeCable.get());
	}
	// Warn user of cable overwrite.
	else
	{
		auto* modal = Lumen::getApp().pushWindow<CircuitDesignerPopupModal>(LumenDockPanel::Floating, "Cable Overwrite");
		modal->m_entityPath = path;
		modal->m_cableOverwrite = true;
		if (loadOnImport)
			modal->m_mousePosition = getMouseLocalPosition();
	}
}

void CircuitDesigner::importComponent(const YAML::Node& node, bool loadOnImport, bool checkForOverwrite)
{
	YAML::Node componentNode = node;
	if (componentNode["Component"].IsDefined()) componentNode = componentNode["Component"];

	// Check if it is a component file.
	std::filesystem::path filename = componentNode["Filename"].as<std::string>();
	if (filename.extension().string() != ".lmcp")
	{
		LUMEN_LOG_WARN("Tried to import component with invalid extension.", "");
		return;
	}

	// Add component if it does not exist.
	std::string componentName = filename.filename().string();
	if (!checkForOverwrite || m_circuit->m_referenceComponents.find(componentName) == m_circuit->m_referenceComponents.end())
	{
		m_circuit->m_referenceComponents.insert({ componentName, componentNode });
		if (loadOnImport) loadAndPlaceComponent(componentNode, getMouseLocalPosition());
	}
	// Warn user of component overwrite.
	else
	{
		auto* modal = Lumen::getApp().pushWindow<CircuitDesignerPopupModal>(LumenDockPanel::Floating, "Cable Overwrite");
		modal->m_yamlNode = true;
		modal->m_node = componentNode;
		modal->m_componentOverwrite = true;
		modal->m_entity = componentName;
		if (loadOnImport)
			modal->m_mousePosition = getMouseLocalPosition();
	}
}

void CircuitDesigner::importCable(const YAML::Node& node, bool loadOnImport, bool checkForOverwrite)
{
	YAML::Node cableNode = node;
	if (cableNode["Cable"].IsDefined()) cableNode = cableNode["Cable"];

	// Check if it is a cable file.
	std::filesystem::path filename(cableNode["Filename"].as<std::string>());
	if (filename.extension().string() != ".lmcb")
	{
		LUMEN_LOG_WARN("Tried to import cable with invalid extension.", "");
		return;
	}

	// Add cable if it does not exist.
	std::string cableName = filename.filename().string();
	if (!checkForOverwrite || m_circuit->m_referenceCables.find(cableName) == m_circuit->m_referenceCables.end())
	{
		m_circuit->m_referenceCables.insert({ cableName, cableNode });
		if (loadOnImport) loadDataToCable(cableNode, m_activeCable.get());
	}
	// Warn user of cable overwrite.
	else
	{
		auto* modal = Lumen::getApp().pushWindow<CircuitDesignerPopupModal>(LumenDockPanel::Floating, "Cable Overwrite");
		modal->m_yamlNode = true;
		modal->m_node = cableNode;
		modal->m_cableOverwrite = true;
		modal->m_entity = cableName;
		if (loadOnImport)
			modal->m_mousePosition = getMouseLocalPosition();
	}
}

int CircuitDesigner::getCableCount(const std::string& type)
{
	// Make sure string has no extension.
	std::string cableType = std::filesystem::path(type).filename().stem().string();
	// Count cables of the type.
	int count = 0;
	for (auto& cable : m_circuit->m_cables)
	{
		if (cable->m_cableType == cableType)
			count++;

	}
	return count;
}

int CircuitDesigner::getComponentCount(const std::string& type) 
{
	// First make sure there is no extension.
	std::string componentType = std::filesystem::path(type).filename().stem().string();
	// Search for components of this type.
	int count = 0;
	for (auto& component : m_circuit->m_components)
	{
		if (component->equipType == componentType)
			count++;
	}
	return count;
}

void CircuitDesigner::deleteComponent(Component2D* component) 
{
	auto iterator = std::find(m_circuit->m_components.begin(), m_circuit->m_components.end(), component);
	if (iterator != m_circuit->m_components.end())
	{
		getGizmo().clearEntities();

		m_circuit->m_components.erase(iterator);
		if (component == m_activeComponent) {
			m_activeComponent = nullptr;
		}
		// Check if there is an hovered port.
		if (!m_hoveredPort) return;
		// Check to see if the hovered port was deleted.
		if (!EntityManager::getEntity(m_hoveredPort->m_entityID))
			m_hoveredPort = nullptr;
	}
}

void CircuitDesigner::removeImportedComponent(const std::string& name, bool checkCount)
{
	// Check if components are to be deleted.
	int count = 0;
	if (checkCount && (count = getComponentCount(name)))
	{
		// Warn user.
		auto* modal = Lumen::getApp().pushWindow<CircuitDesignerPopupModal>(LumenDockPanel::Floating, "Component Delete");
		modal->m_entity = name;
		modal->m_deleteComponents = true;
		modal->m_entityCount = count;
	}
	// Remove entry along with existing components.
	else
	{
		// Ensure the name does not have the file extension.
		std::string componentType = std::filesystem::path(name).filename().stem().string();
		m_circuit->m_referenceComponents.erase(name);
		int count = 0;
		auto& componentsVector = m_circuit->m_components;
		while (count < componentsVector.size())
		{
			Component2D* component = componentsVector[count].get();
			if (component->equipType == componentType)
			{
				if (m_activeComponent.get() == component) m_activeComponent = nullptr;
				componentsVector.erase(componentsVector.begin() + count);
			}
			else count++;
		}
	}
}

void CircuitDesigner::removeImportedCable(const std::string& name, bool checkCount)
{
	// Check if there are cables to remove.
	int count = 0;
	if (checkCount && (count = getCableCount(name)))
	{
		// Warn user.
		auto* modal = Lumen::getApp().pushWindow<CircuitDesignerPopupModal>(LumenDockPanel::Floating, "Clear Cables");
		modal->m_entity = name;
		modal->m_deleteCables = true;
		modal->m_entityCount = count;
	}
	else 
	{
		// Ensure filename does not have an extension.
		std::string cableType = std::filesystem::path(name).filename().stem().string();
		m_circuit->m_referenceCables.erase(name);
		for (auto& cable : m_circuit->m_cables) 
		{
			if (cable->m_cableType == cableType)
			{
				cable->setColour({ 0.f, 0.f, 0.f, 1.f });
				cable->m_cableType = "";
			}
		}
	}
}

void CircuitDesigner::overwriteComponents(const std::string& type, const YAML::Node& node)
{
	// Remove extension.
	std::string componentType = std::filesystem::path(type).filename().stem().string();
	// Overwrite components of same type.
	int overwriteCount = 0;
	for (auto& component : m_circuit->m_components)
	{
		if (component->equipType == componentType)
		{
			reloadComponent(component.get(), node);
			overwriteCount++;
		}
		if (m_activeComponent.get() == component.get()) component->enableOutline();
	}
	if (overwriteCount)
	{
		std::string msg = "Overwritten " + std::to_string(overwriteCount) + " components.";
		Lumen::getApp().pushNotification(NotificationType::Success, 4000, msg, "Circuit Designer");
	}
}

void CircuitDesigner::reloadComponent(Component2D* component, const YAML::Node& node)
{
	// Ensure correct node.
	YAML::Node componentNode = node;
	if (componentNode["Component"].IsDefined()) componentNode = componentNode["Component"];
	glm::vec2 componentCentre = component->centre;
	float componentRotation = component->m_rotation;
	component->centre = {0.f, 0.f};
	component->m_rotation = 0.f;

	// ----------- //
	//  T I T L E  //
	// ----------- //

	// Update the designator.  Technically this will only update the position.
	// Done like this due to numrical instabilities.

	// Equipment type.
	if (node["Title"].IsDefined())
	{
		Renderer::remove(component->title);
		component->title = Renderer::addText2D(node["Title"], component);
		component->equipType = component->title->m_string;
	}
	else if (node["Equipment Type"].IsDefined())
	{
		Renderer::remove(component->title);
		component->title = Renderer::addText2D(node["Equipment Type"], component);
		component->equipType = component->title->m_string;
	}

	// Designator.
	if (node["Designator"].IsDefined())
	{
		Renderer::remove(component->designator);
		component->designator = Renderer::addText2D(node["Designator"], component);
		component->designatorSym = component->designator->m_string;
		component->updateText();
	}

	// ----------------- //
	//  P O L Y G O N S  //
	// ----------------- //

	// Remove existing.
	auto& polygonsVector = component->m_polygons;
	for (auto& poly : polygonsVector) Renderer::remove(poly);
	polygonsVector.clear();
	// Add new.
	for (const auto& poly : componentNode["Polygons"])
			polygonsVector.push_back(Renderer::addPolygon2D(poly.second, component));

	// ----------- //
	//  L I N E S  //
	// ----------- //

	// Remove existing.
	auto& linesVector = component->m_lines;
	for (auto& line : linesVector) Renderer::remove(line);
	linesVector.clear();
	// Add new.
	for (const auto& line : componentNode["PolyLines"])
		linesVector.push_back(Renderer::addPolyLine(line.second, component));

	// --------------- //
	//  C I R C L E S  //
	// --------------- //

	// Remove existing.
	auto& circlesVector = component->m_circles;
	for (auto& circle : circlesVector) Renderer::remove(circle);
	circlesVector.clear();
	// Add new.
	for (const auto& circle : componentNode["Circles"])
		circlesVector.push_back(Renderer::addCircle2D(circle.second, component));

	// --------- //
	//  T E X T  //
	// --------- //

	// Remove existing.
	auto& textVector = component->m_text;
	for (auto& text : textVector) Renderer::remove(text);
	textVector.clear();
	// Add new.
	for (const auto& text : componentNode["Text"])
		textVector.push_back(Renderer::addText2D(text.second, component));

	// ----------- //
	//  P O R T S  //
	// ----------- //

	// Create the new ports.
	std::vector<std::shared_ptr<Port>> newPorts;
	for (const auto& port : componentNode["Ports"])
		newPorts.push_back(std::make_shared<Port>(port.second, component));

	// If any of the new ports are in the position of the old ones,
	// move the cable over.
	auto& portsVector = component->ports;

	// Rotation transform for the port centre.
	glm::mat4 rotation = glm::rotate(glm::mat4(1.f), -glm::radians(componentRotation), {0.f, 0.f, 1.f});

	for (auto& port : portsVector)
	{
		// Reset the port to its original position & rotation.
		glm::vec2 portOriginalCentre = rotation * glm::vec4{ port->centre - componentCentre, 0.f, 1.f };
		// Find port with the same position.
		for (auto& newPort : newPorts)
		{
			// Port has same position.
			if (compare(newPort->centre, portOriginalCentre))
			{
				for (auto cable : port->m_cables)
				{
					if (cable->m_startPort == port.get()) cable->m_startPort = newPort.get();
					if (cable->m_endPort   == port.get()) cable->m_endPort = newPort.get();
					newPort->attachCable(cable);
				}
				port->m_cables.clear();
				break;
			}
		}
	}
	portsVector.clear();
	portsVector = std::move(newPorts);

	// Update component.
	component->move(componentCentre);
	component->rotate(componentRotation);
}

void CircuitDesigner::overwriteCables(const std::string& type, const YAML::Node& node) 
{
	// Remove extension.
	std::string cableType = std::filesystem::path(type).filename().stem().string();
	// Overwrite cables of same type.
	int overwriteCount = 0;
	for (auto& cable : m_circuit->m_cables)
	{
		if (cable->m_cableType == cableType)
		{
			loadDataToCable(node, cable.get());
			overwriteCount++;
		}
	}
	if (overwriteCount)
	{
		std::string msg = "Overwritten " + std::to_string(overwriteCount) + " cables.";
		Lumen::getApp().pushNotification(NotificationType::Success, 4000, msg, "Circuit Designer");
	}
}

bool compare(const glm::vec2& vec1, const glm::vec2& vec2, float tolerance)
{
	return std::abs(vec1.x - vec2.x) < tolerance
		&& std::abs(vec1.y - vec2.y) < tolerance;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//