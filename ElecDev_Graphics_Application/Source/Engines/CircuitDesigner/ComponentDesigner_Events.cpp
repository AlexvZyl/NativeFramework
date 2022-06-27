#include "ComponentDesigner.h"
#include "Application/Events/EventLog.h"
#include "Application/Events/Events.h"
#include "Application/ApplicationTemplates.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "Graphics/OpenGL/Primitives/Polygon.h"
#include "Graphics/OpenGL/Primitives/LineSegment.h"
#include "Graphics/OpenGL/Primitives/Circle.h"
#include "Utilities/Logger/Logger.h"
#include "Graphics/OpenGL/Primitives/PolyLine.h"
#include "GUI/TextEntryGUI/TextEntryGUI.h"
#include "OpenGL/Primitives/Grid.h"
#include "Peripherals/Component2D.h"
#include "OpenGL/SceneGL.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Peripherals/Port.h"

void ComponentDesigner::onMouseButtonEvent(const MouseButtonEvent& event)
{
	if (event.isType(EventType_MousePress | EventType_MouseButtonLeft))
	{
		glm::vec2 pixelCoords = event.mousePosition;
		glm::vec3 WorldCoords = pixelToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };


		if (designerState == CompDesignState::SELECT)
		{
			m_currentEntityID = getEntityID(pixelCoords);
			if (event.isNotType(EventType_LeftCtrl)) {
				// Single selection.
				if (!m_activePrimitives.empty())
				{
					// If we already have an active primitive, we need to check for vertex selection.
					// TODO: Move this to the start of a drag event.
					// TODO: Ideally, we should highlight any hovered vertices as well to indicate possible selection.
					setActiveVertex(screenCoords);

				}
				if (m_activeVertexIdx == -1)
				{
					//If we did not select a vertex, then we can check for a new primitive selection
					setActivePrimitives(m_currentEntityID);
				}
			}
			else {
				//toggle primitives for multiple selection
				toggleActivePrimitives(m_currentEntityID);

			}
		}
		else if (designerState == CompDesignState::DRAW_POLY)
		{

			if (!m_tempPoly && !m_tempLine)
			{
				if (drawFilled)
				{
					m_tempPoly = (Renderer::addPolygon2D({ {getNearestGridVertex(screenCoords), 0.f},{getNearestGridVertex(screenCoords), 0.f} }, penColour, m_activeComponent.get()));
				}
				else {
					m_tempLine = (Renderer::addPolygon2DClear({ getNearestGridVertex(screenCoords),getNearestGridVertex(screenCoords) }, penThickness, m_activeComponent.get(), penColour));
				}
			}
			else
			{
				if(m_tempPoly)
					m_tempPoly->pushVertex({ getNearestGridVertex(screenCoords), 0.f });
				else if (m_tempLine)
					m_tempLine->pushVertex(getNearestGridVertex(screenCoords));
			}
		}
		else if (designerState == CompDesignState::DRAW_LINE)
		{
			if (!m_tempLine)
			{
				//start new line
				bool rounded = false;
				m_tempLine = (Renderer::addPolyLine({ getNearestGridVertex(screenCoords), getNearestGridVertex(screenCoords) }, penThickness, penColour, rounded, m_activeComponent.get()));
			}
			else {
				//end the line
				//m_activeComponent->addLine(m_tempLine);
				pushTempPrimitives();
			}
		}
		else if (designerState == CompDesignState::DRAW_CIRCLE)
		{
			if (!m_tempCircle)
			{
				//start new circle
				if (drawFilled)
				{
					m_tempCircle = (Renderer::addCircle2D(getNearestGridVertex(screenCoords), 0.f, penColour, -1.0f, 0.f, m_activeComponent.get()));
				}
				else
				{
					m_tempCircle = (Renderer::addCircle2D(getNearestGridVertex(screenCoords), 0.f, penColour, penThickness, 0.f, m_activeComponent.get()));
				}
			}
			else
			{
				//add to the component
				pushTempPrimitives();
			}
		}
		else if (designerState == CompDesignState::PLACE_PORT) {
			m_activeComponent->addPort(m_activePort);
			m_activePort = std::make_shared<Port>(getNearestGridVertex(screenCoords), PortType::PORT_INOUT, m_activeComponent.get());
		}
		else if (designerState == CompDesignState::ADD_TEXT)
		{
			// Create a popup GUI for the text entry.
			//if (m_activePrimitives.empty()) {
			Text* activeText = Renderer::addText2D(" ", screenCoords, textColour, textSize, "C", "B", m_activeComponent.get());
			m_activeComponent->m_text.push_back(activeText);

			TextEntryGUI* menu = Lumen::getApp().pushWindow<TextEntryGUI>(LumenDockPanel::Floating, "Text Entry", activeText, &commandLog);
			switchState(CompDesignState::SELECT);
			//}
		}
	}

	if (event.isType(EventType_MousePress | EventType_MouseButtonRight))
	{
		// Create a popup menu on a right click on a graphics scene.
		Lumen::getApp().pushWindow<PopUpMenu>(LumenDockPanel::Floating, "Popup Menu");
	}

	if (event.isType(EventType_MouseDoublePress))
	{
		if (designerState == CompDesignState::SELECT)
		{
			Text* activeText = dynamic_cast<Text*>(m_activePrimitives.at(0));
			if (activeText) Lumen::getApp().pushWindow<TextEntryGUI>(LumenDockPanel::Floating, "Text Entry", activeText, &commandLog);
		}
	}
}

void ComponentDesigner::onMouseMoveEvent(const MouseMoveEvent& event)
{
	glm::vec2 screenCoords = pixelToWorldCoords(event.mousePosition);

	if (designerState == CompDesignState::DRAW_POLY)
	{
		// Move the back vertex.
		if (m_tempPoly)
		{
			m_tempPoly->translateVertexAtIndexTo(m_tempPoly->logicalVertexCount() - 1, getNearestGridVertex(screenCoords));
		}
		if (m_tempLine)
		{
			// Update the line end position.
			m_tempLine->translateVertexAtIndexTo(m_tempLine->logicalVertexCount() - 1, getNearestGridVertex(screenCoords));
		}
	}
	else if (designerState == CompDesignState::DRAW_LINE)
	{
		if (m_tempLine)
		{
			// Update the line end position.
			m_tempLine->translateVertexAtIndexTo(m_tempLine->logicalVertexCount() - 1, getNearestGridVertex(screenCoords));
		}
	}
	else if (designerState == CompDesignState::DRAW_CIRCLE)
	{
		/*TODO: Remove dynamic cast.
		//We could overload translateVertexAtIndexTo() to set the radius of the circle.************
		Circle* activeCircle = dynamic_cast<Circle*>(activeCircle);
		if (activeCircle)
		{
			// Update circle.
			activeCircle->setRadius(glm::length(glm::vec2(activeCircle->m_trackedCenter) - getNearestGridVertex(screenCoords)));
		}
		*/
		if (m_tempCircle)
		{
			// Update circle.
			m_tempCircle->translateVertexAtIndexTo(m_tempCircle->logicalVertexCount() - 1, getNearestGridVertex(screenCoords));
		}

	}
	else if (designerState == CompDesignState::PLACE_PORT)
	{
		if (m_activePort.get()) {
			//update the title location based on the positioning for user convenience
			if (m_activePort->centre.x >= 0 && getNearestGridVertex(screenCoords).x < 0) {
				m_activePort->titleOffset = -m_activePort->titleOffset;
				m_activePort->title->updateAlignment("R");
			}
			if (m_activePort->centre.x < 0 && getNearestGridVertex(screenCoords).x >= 0) {
				m_activePort->titleOffset = -m_activePort->titleOffset;
				m_activePort->title->updateAlignment("L");
			}
			//update port location
			m_activePort->translateTo(getNearestGridVertex(screenCoords));
		}
	}

	else if (designerState == CompDesignState::SELECT)
	{

		if (!m_activePrimitives.empty())
		{
			//If we already have an active primitive, we need to check for vertex selection.
			//TODO: Ideally, we should highlight any hovered vertices as well to indicate possible selection.
			//TODO: Do not highlight text vertices!
			setHoveredVertex(screenCoords);

		}

		if (event.isNotType(EventType_MouseButtonLeft))
		{
			//m_lastDragPos = screenCoords;
		}
	}

	// Store state.
	m_currentEntityID = getEntityID(event.mousePosition);
}

void ComponentDesigner::onMouseScrollEvent(const MouseScrollEvent& event)
{
	Base2DEngine::onMouseScrollEvent(event);
	// Everything is in mm.
	float currentCameraScale = (1.f / getScene().getCamera().getTotalScale().x) * 1000.f;
	float currentGridSize = getScene().getGrid().getCoarseIncrementSize() * 1000.f;
	float gridIncrementFactor = 5.f;
	float minimumGridSize = 1.f;
	if (currentCameraScale < currentGridSize)
	{
		if (currentGridSize / gridIncrementFactor < minimumGridSize)
		{
			getScene().getGrid().setMajorGrid(GridUnit::MILLIMETER, minimumGridSize);
		}
		else
		{
			getScene().getGrid().setMajorGrid(GridUnit::MILLIMETER, currentGridSize / gridIncrementFactor);
		}
	}
	else if (currentCameraScale > currentGridSize * gridIncrementFactor)
	{
		getScene().getGrid().setMajorGrid(GridUnit::MILLIMETER, currentGridSize * gridIncrementFactor);
	}
}

void ComponentDesigner::onKeyEvent(const KeyEvent& event)
{
	// Events based on key type.
	if (event.isType(EventType_KeyPress))
	{
		// Event mouse coordinates.
		glm::vec2 pixelCoords = event.mousePosition;
		glm::vec3 WorldCoords = pixelToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };

		switch (event.key)
		{
		case GLFW_KEY_P:
			//Add new polygon
			switchState(CompDesignState::DRAW_POLY);
			drawFilled = !drawFilled;
			break;

		case GLFW_KEY_L:
			//Add new line
			switchState(CompDesignState::DRAW_LINE);
			break;

		case GLFW_KEY_C:
			//Add new circle
			switchState(CompDesignState::DRAW_CIRCLE);
			drawFilled = !drawFilled;
			break;

		case GLFW_KEY_O:
			//Add new port
			switchState(CompDesignState::PLACE_PORT);
			break;

		case GLFW_KEY_ESCAPE:
			if (m_tempCircle || m_tempLine || m_tempPoly) {
				switchState(designerState);
			}
			else { switchState(CompDesignState::SELECT); }//Consideration: Do we want to do this when we are already in the designer state?
			break;

		case GLFW_KEY_S:
			/*if (!m_activePoly) return;
			glm::vec2 center = m_activePoly->m_trackedCenter;
			glm::vec2 centerGlobalPixelCoords = localToGlobalCoords(worldToPixelCoords(center));
			EventLog::logMouseScroll(centerGlobalPixelCoords, 1.f, 0.f, 0);
			EventLog::logMouseDrag(centerGlobalPixelCoords, centerGlobalPixelCoords + glm::vec2{ 100.f, 0.f }, { 100.f, 0.f }, EventType_MouseButtonLeft);
			glm::vec2 expectedPos = center + glm::vec2(pixelToWorldDistance({100.f, 0.f}));
			Renderer::addCircle2D(glm::vec3{ glm::vec2(expectedPos), 0.6f }, 0.01f, { 1.f, 0.f, 1.f, 1.f });*/
			// Red = tracked center
			// Pink = expected position
			break;


		case GLFW_KEY_T:
			//Add new text
			switchState(CompDesignState::ADD_TEXT);
			break;

		case GLFW_KEY_DELETE:
			if (designerState == CompDesignState::SELECT) deleteActivePrimitive();
			break;

		case GLFW_KEY_BACKSPACE:
			if (designerState == CompDesignState::SELECT) deleteActivePrimitive();
			break;

		case GLFW_KEY_ENTER:
			if (designerState != CompDesignState::SELECT) {
				pushTempPrimitives();
				switchState(CompDesignState::SELECT);
			}
			break;
		}
	}
}

void ComponentDesigner::onMouseDragEvent(const MouseDragEvent& event)
{
	Base2DEngine::onMouseDragEvent(event);
	if (designerState == CompDesignState::SELECT) {
		if (event.isType(EventType_MouseButtonLeft) && event.isNotType(EventType_MouseButtonLeft | EventType_SpaceBar))
		{
			glm::vec2 pixelCoords = event.mousePosition;
			glm::vec3 WorldCoords = pixelToWorldCoords(pixelCoords);
			glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };
			if (designerState == CompDesignState::SELECT)
			{
				// User is dragging a component.
				glm::vec2 translation = pixelToWorldDistance(event.currentFrameDelta);
				if (m_activeVertexIdx != -1)
				{
					// First check if we should move a vertex
					// We need to move a vertex

					//TODO:Proper support for all types
					//This is only properly supported for Polygons (and childeren including polylines)

					Polygon2D* activePoly = dynamic_cast<Polygon2D*>(activeVertexOwner);
					if (activePoly)
						activePoly->translateVertexAtIndex(m_activeVertexIdx, translation);

					//Circles have support, but still should be handled differently (we only have 'translateVertexAtIndexTo()' to work with)
					Circle* activeCircle = dynamic_cast<Circle*>(activeVertexOwner);
					if (activeCircle)
						activeCircle->translateVertexAtIndexTo(m_activeVertexIdx, getNearestGridVertex(screenCoords));

				}
				else
				{
					// If we are not moving a vertex, then check to move primitives
					if (!m_activePrimitives.empty())
					{
						for (auto primitive : m_activePrimitives) {
							primitive->translate(translation);
						}
						//m_lastDragPos = screenCoords;
					}
					if (m_activePort.get())
					{
						m_activePort->translate(translation);
						//m_lastDragPos = getNearestGridVertex(screenCoords);
					}
				}
			}
		}
	}
}

void ComponentDesigner::onNotifyEvent(const NotifyEvent& event)
{
	if (designerState == CompDesignState::SELECT) {
		if (event.isType(EventType_MouseDragStart | EventType_MouseButtonLeft))
		{
			//retain the starting positions of any primitives before they are dragged
			//TODO: This will be obsolete once the undo system can properly correlate/combine sucessive translation commands to the same entity.
			//clear drag start points
			m_dragStart.clear();
			//add start points for vertices
			if (m_activeVertexIdx != -1) {
				if (activeVertexOwner) m_dragStart.push_back(activeVertexOwner->getLogicalVertex(m_activeVertexIdx));
			}
			//add start points for primitives and ports
			else {
				for (int i = 0; i < m_activePrimitives.size(); i++) {
					m_dragStart.push_back(m_activePrimitives.at(i)->m_trackedCenter);
				}
				if (m_activePort) {
					m_dragStart.push_back(m_activePort->centre);
				}
			}
		}
		else if (event.isType(EventType_MouseDragStop | EventType_MouseButtonLeft))
		{

			if (m_activeVertexIdx == -1) {
				//move primitives
				for (int i = 0; i < m_activePrimitives.size(); i++) {
					m_activePrimitives.at(i)->translateTo(getNearestGridVertex(m_activePrimitives.at(i)->m_trackedCenter));
					commandLog.log<Translate2DCommand>(glm::vec2{ m_activePrimitives.at(i)->m_trackedCenter } - m_dragStart.at(i), m_activePrimitives.at(i));
				}
				if (m_activePort) {
					m_activePort->translateTo(getNearestGridVertex(m_activePort->centre));
					commandLog.log<Translate2DCommand>(glm::vec2{ m_activePort->centre } - m_dragStart.front(), m_activePort.get());
				}
			}
			else {
				//move vertices

				//Again, this is not yet fully supported for all primitives. For now, we need to do some dynamic casts
				Polygon2D* activePoly = dynamic_cast<Polygon2D*>(activeVertexOwner);
				if (activePoly) {
					activePoly->translateVertexAtIndexTo(m_activeVertexIdx, getNearestGridVertex(activePoly->getLogicalVertex(m_activeVertexIdx)));
					commandLog.log<TranslateVertexCommand>(m_activeVertexIdx, activePoly->getLogicalVertex(m_activeVertexIdx) - m_dragStart.front(), activePoly);
				}

				//Circles have support, but still should be handled differently (we only have 'translateVertexAtIndexTo()' to work with)
				Circle* activeCircle = dynamic_cast<Circle*>(activeVertexOwner);
				if (activeCircle) {
					//Circles currently snap to grid coordinates, so it is not necessary to adjust the radius again here.
					//activeCircle->translateVertexAtIndexTo(m_activeVertexIdx, getNearestGridVertex(screenCoords)); 

					//Circle radius changes are logged, but currently buggy due to the incorrect behaviour of getLogicalVertex();
					commandLog.log<TranslateVertexCommand>(m_activeVertexIdx, activeCircle->getLogicalVertex(m_activeVertexIdx) - m_dragStart.front(), activeCircle);
				}

			}
		}
	}
}

void ComponentDesigner::onFileSaveEvent(const FileSaveEvent& event)
{
	// Save to existing file.
	if (event.saveAs)
	{
		// Iterate through the paths.
		for (auto& path : event.fileData)
		{
			// Check if operation did not fail.
			if (path.string().size())
			{
				saveToYAML(m_activeComponent.get(), path);
				setName(path.filename().stem().string());
				if (path.extension().string() != ".lmcp")
				{
					path.extension() = ".lmcp";
				}
				savePath = path;
				savedDocument();
			}
		}
	}
	// Save to new file.
	else {
		if (!savePath.string().size()) {
			// Get save path.
			savePath = selectFile("Lumen Save Circuit", "", m_activeComponent->title->m_string, "Save");
			if (savePath.extension().string() != ".lmcp")
			{
				savePath.extension() = ".lmcp";
			}
		}
		if (savePath.string().size())
		{
			saveToYAML(m_activeComponent.get(), savePath);
			setName(savePath.filename().stem().string());
			savedDocument();
		}
	}
}