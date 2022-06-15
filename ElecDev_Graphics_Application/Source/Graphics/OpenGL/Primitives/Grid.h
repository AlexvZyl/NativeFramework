#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "glm/glm.hpp"
#include "glad/glad.h"
#include <memory>
#include "OpenGL/Primitives/Circle.h"
#include "imgui/imgui.h"
#include "Utilities/Logger/Logger.h"
#include "Graphics/Camera/Camera.h"
#include "glm/gtc/matrix_transform.hpp"

//=============================================================================================================================================//
//  Forward decleration.																													   //
//=============================================================================================================================================//

class VertexArrayObject;

class VertexData;
struct IndexData2;

//=============================================================================================================================================//
//  Circle class.		 																													   //
//=============================================================================================================================================//

// 1 Unit in world coordinates corresponds to 1 meter.
// Subject to change.

enum class GridUnit
{
	MILLIMETER,
	CENTIMETER,
	METER,
	KILOMETER,
	INCH,
	FOOT,
	MILE
};

enum class GridWidgetPosition 
{
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

class Grid
{
public:

	// Constructor.
	Grid();
	// Destructor.
	~Grid();

	// Setters
	inline Grid& setFinePixelSize(int size) 
	{
		m_fineGridPixelSize = size;
		return *this;
	}
	inline Grid& setCoarsePixelSize(int size)
	{
		m_coarseGridPixelSize = size;
		return *this;
	}
	inline Grid& setOriginPixelSize(int size) 
	{
		m_originGridPixelSize = size;
		return *this;
	}
	inline Grid& setFineColor(const glm::vec4& color)
	{
		m_fineGridColor = color;
		return *this;
	}
	inline Grid& setCoarseColor(const glm::vec4& color)
	{
		m_coarseGrid = color;
		return *this;
	}
	inline Grid& setTotalCoarseLines(int total)
	{
		m_totalCoarseLines = total;
		return *this;
	}
	inline Grid& enable()
	{
		if (m_helperCircleEnabled) visibleHelperCircle();
		m_enabled = true;
		return *this;
	}
	inline Grid& disable()
	{
		hideHelperCircle();
		m_enabled = false;
		return *this;
	}
	inline Grid& setFineIncrementSize(float size) 
	{
		m_fineIncrementSize = size;
		return *this;
	}
	inline Grid& setCoarseIncrementSize(float size)
	{
		m_coarseIncrementSize = size;
		return *this;
	}
	inline Grid& setHelperCircleColor(const glm::vec4& color) 
	{
		m_helperCircleColor = color;
		return *this;
	}
	inline Grid& enableHelperCircle() 
	{
		m_helperCircleEnabled = true;
		visibleHelperCircle();
		return *this;
	}
	inline Grid& disableHelperCircle() 
	{
		m_helperCircleEnabled = false;
		hideHelperCircle();
		return *this;
	}
	inline Grid& setScale(float scale)
	{
		m_scale = scale;
		return *this;
	}
	inline float getScale()
	{
		return m_scale;
	}
	glm::mat4 getViewProjectionMatrix(Camera& camera) 
	{
		glm::mat4 viewMatrix = glm::scale(glm::mat4(1.f), {m_scale, m_scale, 1.f});
		glm::vec3 scaledPosition = camera.m_position;
		scaledPosition.x /= m_scale;
		scaledPosition.y /= m_scale;

		switch (camera.getType())
		{
		case CameraType::Standard2D:
			return camera.getProjectionMatrix() * camera.m_scalingMatrix * camera.m_rotationMatrix * glm::translate(viewMatrix, scaledPosition);
			break;

		case CameraType::Standard3D:
			return glm::mat4(0.f);;
			break;

		default:
			LUMEN_LOG_WARN("Unknown camera type.", "Grid");
			return glm::mat4(0.f);;
			break;
		}
	}
	inline Grid& setWidgetPosition(GridWidgetPosition position)
	{
		m_widgetPosition = position;	
		return *this;
	}
	inline Grid& setWidgetSize(const glm::vec2& size) 
	{
		m_widgetSize = size;
		return *this;
	}
	inline Grid& renderOverlay()
	{
		// Setup.
		glm::vec2 currentCursorPos = ImGui::GetCursorPos();
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
		glm::vec2 contentWindowRegionSize = ImGui::GetWindowContentRegionMax();
		switch (m_widgetPosition)
		{
		case GridWidgetPosition::BOTTOM_RIGHT:
			ImGui::SetCursorPos(contentWindowRegionSize - m_widgetPadding - m_widgetSize);
			break;

		default:
			LUMEN_LOG_WARN("Invalid grid widget position.", "Grid");
			break;
		}

		// Render the widget.
		if (ImGui::BeginChild("GridWidget", m_widgetSize, true))
		{
			std::string message;
			if(m_scale * 1000.f < 1000.f) message = "Major Grid = " + std::to_string(m_coarseIncrementSize * m_scale * 1000) + " mm";
			else					      message = "Major Grid = " + std::to_string(m_coarseIncrementSize * m_scale) + " m";
			glm::vec2 size = ImGui::CalcTextSize(message.c_str());
			setWidgetSize(size + m_widgetInternalPadding);
			ImGui::SetCursorPos({
					m_widgetSize.x / 2 - size.x / 2,
					m_widgetSize.y / 2 - size.y / 2,
				});
			ImGui::Text(message.c_str());
		}
		ImGui::EndChild();

		// Cleanup.
		ImGui::PopStyleVar();
		ImGui::SetCursorPos(currentCursorPos);
		return *this;
	}
	inline Grid& setMajorGrid(GridUnit unit, float value)
	{
		switch (unit)
		{
		case GridUnit::MILLIMETER:
			setScale(value / 1000.f);
			break;
		case GridUnit::CENTIMETER:
			setScale(value / 100.f);
			break;
		case GridUnit::METER:
			setScale(value);
			break;
		case GridUnit::KILOMETER:
			setScale(value * 1000.f);
			break;
		default:
			LUMEN_LOG_WARN("Invalid GridUnit.", "Grid");
		}
		return *this;
	}

	// Getters.
	inline int getFinePixelSize()			{ return m_coarseGridPixelSize; }
	inline int getCoarsePixelSize()			{ return m_fineGridPixelSize; }
	inline int getOriginPixelSize()			{ return m_originGridPixelSize; }
	inline float getCoarseIncrementSize()	{ return m_coarseIncrementSize * m_scale; }
	inline float getFineIncrementSize()		{ return m_fineIncrementSize * m_scale;}
	inline glm::vec4& getFineColor()		{ return m_fineGridColor; }
	inline glm::vec4& getCoarseColor()		{ return m_coarseGridColor; }
	inline int getTotalCoarseLines()		{ return m_totalCoarseLines; }
	inline bool isEnabled()					{ return m_enabled;	}
	inline glm::vec4& getHelperCircleColor(){ return m_helperCircleColor; }

	// Utility functions.
	void updateHelperCircle(const glm::vec2& coords);
	Grid& createGrid();
	Grid& destroyGrid();
	// Find the grid vertex closest to the given coordinates.
	glm::vec2 getNearestGridVertex(const glm::vec2& coords);
	// This function should be called when data has been changed and needs to be updated.
	//  TODO!
	inline void updateGridData(){}

private:

	friend class Renderer;
	friend class EngineCore;

	// Grid data.
	int m_fineGridPixelSize = 1;
	int m_coarseGridPixelSize = 2;
	int m_originGridPixelSize = 3;
	float m_coarseIncrementSize = 1.f;
	float m_fineIncrementSize = 0.1f;
	glm::vec4 m_coarseGrid = { 1.f, 1.f, 1.f, 1.f };
	glm::vec4 m_coarseGridColor = { 1.f, 1.f, 1.f, 0.9f };
	glm::vec4 m_fineGridColor = { 1.f, 1.f, 1.f, 0.2f };
	glm::vec4 m_xAxisColor = { 0.f, 1.f, 0.f, 1.f };
	glm::vec4 m_yAxisColor = {1.f, 0.f, 0.f, 1.f};
	glm::vec4 m_helperCircleColor = {0.f, 0.f, 1.f, 1.f};
	int m_totalCoarseLines = 300;
	bool m_enabled = true;
	bool m_helperCircleEnabled = true;
	GridWidgetPosition m_widgetPosition = GridWidgetPosition::BOTTOM_RIGHT;
	glm::vec2 m_widgetSize = { 100.f, 50.f };
	glm::vec2 m_widgetPadding = { 5.f,5.f };
	glm::vec2 m_widgetInternalPadding = {20.f, 20.f}; 
	float m_scale = 1.f;

	// VAO containing grid vertices.
	std::unique_ptr<GraphicsLinesBuffer<VertexData>> m_fineBuffer = nullptr;
	std::unique_ptr<GraphicsLinesBuffer<VertexData>> m_coarseBuffer = nullptr;
	std::unique_ptr<GraphicsLinesBuffer<VertexData>> m_originBuffer = nullptr;

	// Circle used to identify active vertex.
	Circle* m_helperCircle = nullptr;

	// Internal utility functions.
	inline Grid& hideHelperCircle()
	{
		m_helperCircle->setColor({ 0.f, 0.f, 0.f, 0.f, });
		return *this;
	}
	inline Grid& visibleHelperCircle()
	{
		m_helperCircle->setColor(m_helperCircleColor);
		return *this;
	}
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
