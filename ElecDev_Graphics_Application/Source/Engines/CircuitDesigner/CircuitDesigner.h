#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/Base2DEngine/Base2DEngine.h"
#include <iostream>
#include "yaml-cpp/yaml.h"
#include <filesystem>
#include <unordered_map>

//=============================================================================================================================================//
//  Forward declerations																													   //
//=============================================================================================================================================//

class Circle;
class Polygon2D;
class Component2D;
class Circuit;
class Cable;
class Port;

//=============================================================================================================================================//
//  Design 2D Engine class.																													   //
//=============================================================================================================================================//

// Design engine states.
enum designState 
{ 
	COMPONENT_PLACE, 
	ENTITY_SELECT, 
	COMPONENT_MOVE, 
	CABLE_PLACE
};

class CircuitDesigner : public Base2DEngine
{

public:

	// Constructor
	CircuitDesigner();
	// Destructor.
	virtual ~CircuitDesigner() override;
	
	// ------------------ //
	//  V A R I A B L E S //
	// ------------------ //

	inline static CircuitDesigner* s_engineUsedByCircuitEditor = nullptr;
	designState designerState = ENTITY_SELECT;
	std::shared_ptr<Component2D> m_activeComponent;
	std::shared_ptr<Cable> m_activeCable;
	unsigned m_activeVertexIdx = -1;
	std::shared_ptr<Circuit> m_circuit;
	glm::vec2 m_lastDragPos = {0.f, 0.f};
	unsigned int m_currentEntityID = 0;
	Port* m_hoveredPort = nullptr;
	unsigned m_hoveredID;
	float clickTol = 15.0f;

	// Events.
	virtual void onMouseButtonEvent(const MouseButtonEvent& event) override;
	virtual void onMouseMoveEvent(const MouseMoveEvent& event) override;
	virtual void onMouseScrollEvent(const MouseScrollEvent& event) override;
	virtual void onMouseDragEvent(const MouseDragEvent& event) override;
	virtual void onKeyEvent(const KeyEvent& event) override;
	virtual void onFileDropEvent(const FileDropEvent& event) override;
	virtual void onNotifyEvent(const NotifyEvent& event) override;
	virtual void onYamlNodeDropEvent(const YamlNodeDropEvent& event) override;
	virtual void onFileSaveEvent(const FileSaveEvent& event) override;

	// Utilities.
	void setActiveComponent(unsigned eID);
	void setActiveCable(unsigned eID);
	void ComponentPlaceMode(glm::vec2 screenCoords);
	void ComponentPlaceMode();
	void deleteActiveComponent();
	void deleteActiveCable();
	Port* getPort(unsigned eID);
	virtual void setNameOfElements(const std::string& name) override;
	void createCircuit(const std::filesystem::path& path);
	void loadAndPlaceComponent(const std::filesystem::path& path, const glm::vec2& mousePos);
	void loadAndPlaceComponent(const YAML::Node& node, const glm::vec2& mousePos);
	void loadDataToCable(const YAML::Node& node, Cable* cable);
	int getComponentCount(const std::string& type);
	int getCableCount(const std::string& type);
	void deleteComponent(Component2D* component);
	void importComponent(const std::filesystem::path& name, bool loadOnImport = true, bool checkForOverwrite = true);
	void importComponent(const YAML::Node& node, bool loadOnImport = true, bool checkForOverwrite = true);
	void importCable(const std::filesystem::path& name, bool loadOnImport = true, bool checkForOverwrite = true);
	void importCable(const YAML::Node& node, bool loadOnImport = true, bool checkForOverwrite = true);
	void removeImportedComponent(const std::string& component, bool checkCount = true);
	void removeImportedCable(const std::string& cable, bool checkCount = true);
	void overwriteComponents(const std::string& type, const YAML::Node& node);
	void overwriteCables(const std::string& type, const YAML::Node& node);
	void reloadComponent(Component2D* component, const YAML::Node& node);

	virtual void renderOverlay() override;
	virtual void renderMenuBar() override;

private:

	bool m_gizmoEnabled = true;
	friend class CircuitEditor;
	friend class CircuitDesignerPopupModal;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//