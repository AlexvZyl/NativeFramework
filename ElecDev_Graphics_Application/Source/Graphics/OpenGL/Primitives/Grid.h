#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "glm/glm.hpp"
#include "glad/glad.h"
#include <memory>
#include "OpenGL/Primitives/Circle.h"

//=============================================================================================================================================//
//  Forward decleration.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;

class VertexData;

//=============================================================================================================================================//
//  Circle class.		 																													   //
//=============================================================================================================================================//

class Grid
{
public:

	Grid();
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

	void updateHelperCircle(const glm::vec2& coords);
	Grid& createGrid();
	Grid& destroyGrid();

	// Getters.
	inline int getFinePixelSize()			{ return m_coarseGridPixelSize; }
	inline int getCoarsePixelSize()			{ return m_fineGridPixelSize; }
	inline int getOriginPixelSize()			{ return m_originGridPixelSize; }
	inline float getCoarseIncrementSize()	{ return m_coarseIncrementSize; }
	inline float getFineIncrementSize()		{ return m_fineIncrementSize;}
	inline glm::vec4& getFineColor()		{ return m_fineGridColor; }
	inline glm::vec4& getCoarseColor()		{ return m_coarseGridColor; }
	inline int getTotalCoarseLines()		{ return m_totalCoarseLines; }
	inline bool isEnabled()					{ return m_enabled;	}
	inline glm::vec4& getHelperCircleColor(){ return m_helperCircleColor; }

	// Find the grid vertex closest to the given coordinates.
	glm::vec2 getClosestGridVertex(const glm::vec2& coords);

private:

	friend class Renderer;

	// Grid data.
	int m_fineGridPixelSize = 1;
	int m_coarseGridPixelSize = 2;
	int m_originGridPixelSize = 3;
	float m_coarseIncrementSize = 0.2f;
	float m_fineIncrementSize = 0.04f;
	glm::vec4 m_coarseGrid = { 1.f, 1.f, 1.f, 1.f };
	glm::vec4 m_coarseGridColor = { 1.f, 1.f, 1.f, 0.9f };
	glm::vec4 m_fineGridColor = { 1.f, 1.f, 1.f, 0.3f };
	glm::vec4 m_xAxisColor = { 0.f, 1.f, 0.f, 1.f };
	glm::vec4 m_yAxisColor = {1.f, 0.f, 0.f, 1.f};
	glm::vec4 m_helperCircleColor = {0.f, 0.f, 1.f, 1.f};
	int m_totalCoarseLines = 10;
	bool m_enabled = true;
	bool m_helperCircleEnabled = true;

	// VAO containing grid vertices.
	std::unique_ptr<VertexArrayObject<VertexData>> m_fineVAO = nullptr;
	std::unique_ptr<VertexArrayObject<VertexData>> m_coarseVAO = nullptr;
	std::unique_ptr<VertexArrayObject<VertexData>> m_originVAO = nullptr;

	// Circle used to identify active vertex.
	Circle* m_helperCircle = nullptr;

};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
