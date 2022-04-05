//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Grid.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Primitives/Circle.h"
#include "OpenGL/Renderer/RendererGL.h"

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

Grid::Grid() 
{
	// Setup buffers.
	m_fineVAO	= std::make_unique<VertexArrayObject<VertexData>>(GL_LINES);
	m_coarseVAO = std::make_unique<VertexArrayObject<VertexData>>(GL_LINES);
	m_originVAO = std::make_unique<VertexArrayObject<VertexData>>(GL_LINES);
	// Add the circle used to display the nearest vertex.
	m_helperCircle = Renderer::addCircle2D({ 0.f, 0.f, -0.99999f }, m_fineIncrementSize / 5.f, m_helperCircleColor);
	createGrid();
}

Grid::~Grid() 
{
	Renderer::remove(m_helperCircle);
}

glm::vec2 Grid::getClosestGridVertex(const glm::vec2& coords) 
{
	if (m_enabled)
	{
		glm::vec2 nearestVertex;
		nearestVertex.x = std::round(coords.x / m_fineIncrementSize) * m_fineIncrementSize;
		nearestVertex.y = std::round(coords.y / m_fineIncrementSize) * m_fineIncrementSize;
		return nearestVertex;
	}
	return coords;
}

void Grid::updateHelperCircle(const glm::vec2& coords) 
{
	m_helperCircle->translateTo(getClosestGridVertex(coords));
}

Grid& Grid::createGrid() 
{
	enableHelperCircle();

	// Calculate total required vertices.
	float maxVertexCoord = (float)m_totalCoarseLines * (float)m_coarseIncrementSize;
	int totalCoarseVerts = m_totalCoarseLines * 2 * 4;
	int totalFineVerts = (int)std::floor(maxVertexCoord / m_fineIncrementSize) * 2 * 8;
	m_fineVAO->m_vertexCPU.reserve(totalFineVerts);
	m_fineVAO->m_indexCPU.reserve(totalFineVerts);
	m_coarseVAO->m_vertexCPU.reserve(totalCoarseVerts);
	m_coarseVAO->m_indexCPU.reserve(totalCoarseVerts);

	// ------------- //
	//  O R I G I N  //
	// ------------- //

	m_originVAO->m_vertexCPU.reserve(4);
	m_originVAO->m_indexCPU.reserve(4);
	m_originVAO->setBufferIncrementSize(4);
	m_originVAO->m_vertexCPU.emplace_back(VertexData({ 0.f, maxVertexCoord, -1.f }, m_xAxisColor, 0));
	m_originVAO->m_vertexCPU.emplace_back(VertexData({ 0.f, -maxVertexCoord, -1.f }, m_xAxisColor, 0));
	m_originVAO->m_vertexCPU.emplace_back(VertexData({ maxVertexCoord, 0.f, -1.f }, m_yAxisColor, 0));
	m_originVAO->m_vertexCPU.emplace_back(VertexData({ -maxVertexCoord, 0.f, -1.f }, m_yAxisColor, 0));
	m_originVAO->m_indexCPU.insert(m_originVAO->m_indexCPU.begin(), { 0,1, 2,3 });
	m_originVAO->m_indexBufferSynced = false;
	m_originVAO->m_vertexBufferSynced = false;
	m_originVAO->m_vertexCount = 4;
	m_originVAO->m_indexCount = 4;
	m_originVAO->queryBufferResize();

	// ------------------------------- //
	//  C O A R S E   V E R T I C E S  //
	// ------------------------------- //

	float currentPosition = m_coarseIncrementSize;
	unsigned vertexCount = 0;
	while (currentPosition <= maxVertexCoord + m_coarseIncrementSize)
	{
		// X Positive Lines.
		m_coarseVAO->m_vertexCPU.emplace_back(VertexData({ currentPosition, maxVertexCoord, -1.f }, m_coarseGridColor, 0));
		m_coarseVAO->m_vertexCPU.emplace_back(VertexData({ currentPosition, -maxVertexCoord, -1.f }, m_coarseGridColor, 0));
		m_coarseVAO->m_indexCPU.insert(m_coarseVAO->m_indexCPU.end(),
			{
				0 + vertexCount, 1 + vertexCount
			});
		vertexCount += 2;

		// X Negative Lines.
		m_coarseVAO->m_vertexCPU.emplace_back(VertexData({ -currentPosition, maxVertexCoord, -1.f }, m_coarseGridColor, 0));
		m_coarseVAO->m_vertexCPU.emplace_back(VertexData({ -currentPosition, -maxVertexCoord, -1.f }, m_coarseGridColor, 0));
		m_coarseVAO->m_indexCPU.insert(m_coarseVAO->m_indexCPU.end(),
			{
				0 + vertexCount, 1 + vertexCount
			});
		vertexCount += 2;

		// Y Positive Lines.
		m_coarseVAO->m_vertexCPU.emplace_back(VertexData({ maxVertexCoord, currentPosition, -1.f }, m_coarseGridColor, 0));
		m_coarseVAO->m_vertexCPU.emplace_back(VertexData({ -maxVertexCoord, currentPosition, -1.f }, m_coarseGridColor, 0));
		m_coarseVAO->m_indexCPU.insert(m_coarseVAO->m_indexCPU.end(),
			{
				0 + vertexCount, 1 + vertexCount
			});
		vertexCount += 2;

		// Y Negative Lines.
		m_coarseVAO->m_vertexCPU.emplace_back(VertexData({ maxVertexCoord, -currentPosition, -1.f }, m_coarseGridColor, 0));
		m_coarseVAO->m_vertexCPU.emplace_back(VertexData({ -maxVertexCoord, -currentPosition, -1.f }, m_coarseGridColor, 0));
		m_coarseVAO->m_indexCPU.insert(m_coarseVAO->m_indexCPU.end(),
			{
				0 + vertexCount, 1 + vertexCount
			});
		vertexCount += 2;

		currentPosition += m_coarseIncrementSize;
	}

	m_coarseVAO->m_indexBufferSynced = false;
	m_coarseVAO->m_vertexBufferSynced = false;
	m_coarseVAO->m_vertexCount = vertexCount;
	m_coarseVAO->m_indexCount = vertexCount;
	m_coarseVAO->queryBufferResize();

	// --------------------------- //
	//  F I N E   V E R T I C E S  //
	// --------------------------- //

	currentPosition = m_fineIncrementSize;
	vertexCount = 0;
	while (currentPosition <= maxVertexCoord)
	{
		// X Positive Lines.
		m_fineVAO->m_vertexCPU.emplace_back(VertexData({ currentPosition, maxVertexCoord, -1.f }, m_fineGridColor, 0));
		m_fineVAO->m_vertexCPU.emplace_back(VertexData({ currentPosition, -maxVertexCoord, -1.f }, m_fineGridColor, 0));
		m_fineVAO->m_indexCPU.insert(m_fineVAO->m_indexCPU.end(),
			{
				0 + vertexCount, 1 + vertexCount
			});
		vertexCount += 2;

		// X Negative Lines.
		m_fineVAO->m_vertexCPU.emplace_back(VertexData({ -currentPosition, maxVertexCoord, -1.f }, m_fineGridColor, 0));
		m_fineVAO->m_vertexCPU.emplace_back(VertexData({ -currentPosition, -maxVertexCoord, -1.f }, m_fineGridColor, 0));
		m_fineVAO->m_indexCPU.insert(m_fineVAO->m_indexCPU.end(),
			{
				0 + vertexCount, 1 + vertexCount
			});
		vertexCount += 2;

		// Y Positive Lines.
		m_fineVAO->m_vertexCPU.emplace_back(VertexData({ maxVertexCoord, currentPosition, -1.f }, m_fineGridColor, 0));
		m_fineVAO->m_vertexCPU.emplace_back(VertexData({ -maxVertexCoord, currentPosition, -1.f }, m_fineGridColor, 0));
		m_fineVAO->m_indexCPU.insert(m_fineVAO->m_indexCPU.end(),
			{
				0 + vertexCount, 1 + vertexCount
			});
		vertexCount += 2;

		// Y Negative Lines.
		m_fineVAO->m_vertexCPU.emplace_back(VertexData({ maxVertexCoord, -currentPosition, -1.f }, m_fineGridColor, 0));
		m_fineVAO->m_vertexCPU.emplace_back(VertexData({ -maxVertexCoord, -currentPosition, -1.f }, m_fineGridColor, 0));
		m_fineVAO->m_indexCPU.insert(m_fineVAO->m_indexCPU.end(),
			{
				0 + vertexCount, 1 + vertexCount
			});
		vertexCount += 2;

		currentPosition += m_fineIncrementSize;
	}

	m_fineVAO->m_indexBufferSynced = false;
	m_fineVAO->m_vertexBufferSynced = false;
	m_fineVAO->m_vertexCount = vertexCount;
	m_fineVAO->m_indexCount = vertexCount;
	m_fineVAO->queryBufferResize();

	return *this;
}

Grid& Grid::destroyGrid() 
{
	disableHelperCircle();
	m_coarseVAO->wipeAll();
	m_fineVAO->wipeAll();
	m_originVAO->wipeAll();
	return *this;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//