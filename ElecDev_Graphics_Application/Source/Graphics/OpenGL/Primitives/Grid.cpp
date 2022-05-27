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
	m_fineVAO	= std::make_unique<VertexArrayObject<VertexData, IndexData2>>(GL_LINES);
	m_coarseVAO = std::make_unique<VertexArrayObject<VertexData, IndexData2>>(GL_LINES);
	m_originVAO = std::make_unique<VertexArrayObject<VertexData, IndexData2>>(GL_LINES);
	// Add the circle used to display the nearest vertex.
	m_helperCircle = Renderer::addCircle2D({ 0.f, 0.f, -0.99999f }, m_fineIncrementSize / 5.f, m_helperCircleColor);
	createGrid();
}

Grid::~Grid() 
{
	Renderer::remove(m_helperCircle);
}

glm::vec2 Grid::getNearestGridVertex(const glm::vec2& coords) 
{
	if (m_enabled)
	{
		float reference = m_fineIncrementSize * m_scale;
		return { std::round(coords.x / reference) * reference,
				 std::round(coords.y / reference) * reference };
	}
	return coords;
}

void Grid::updateHelperCircle(const glm::vec2& coords) 
{
	m_helperCircle->translateTo(getNearestGridVertex(coords));
}

Grid& Grid::createGrid()
{
	// Calculate total required vertices.
	float maxVertexCoord = (float)m_totalCoarseLines * (float)m_coarseIncrementSize;
	int totalCoarseVerts = m_totalCoarseLines * 2 * 4;
	int totalFineVerts = (int)std::floor(maxVertexCoord / m_fineIncrementSize) * 2 * 8;
	// Setup the VAO's.
	m_fineVAO->setCapacityIncrements(totalFineVerts);
	m_fineVAO->setCapacityIncrements(totalFineVerts);
	m_coarseVAO->setCapacityIncrements(totalCoarseVerts);
	m_coarseVAO->setCapacityIncrements(totalCoarseVerts);
	m_originVAO->setCapacityIncrements(4);
	std::vector<VertexData> vertices;
	vertices.reserve(totalFineVerts);
	std::vector<IndexData2> indices;

	// ------------- //
	//  O R I G I N  //
	// ------------- //

	vertices.emplace_back(VertexData({ 0.f, maxVertexCoord, -1.f }, m_xAxisColor, 0));
	vertices.emplace_back(VertexData({ 0.f, -maxVertexCoord, -1.f }, m_xAxisColor, 0));
	vertices.emplace_back(VertexData({ maxVertexCoord, 0.f, -1.f }, m_yAxisColor, 0));
	vertices.emplace_back(VertexData({ -maxVertexCoord, 0.f, -1.f }, m_yAxisColor, 0));
	m_originVAO->push(vertices);
	indices.emplace_back(IndexData2(0, 1));
	indices.emplace_back(IndexData2(2, 3));
	m_originVAO->push(indices);
	vertices.clear();
	indices.clear();

	// ------------------------------- //
	//  C O A R S E   V E R T I C E S  //
	// ------------------------------- //

	float currentPosition = m_coarseIncrementSize;
	unsigned vertexCount = 0;
	while (currentPosition <= maxVertexCoord + m_coarseIncrementSize)
	{
		// X Positive Lines.
		vertices.emplace_back(VertexData({ currentPosition, maxVertexCoord, -1.f }, m_coarseGridColor, 0));
		vertices.emplace_back(VertexData({ currentPosition, -maxVertexCoord, -1.f }, m_coarseGridColor, 0));
		indices.emplace_back(IndexData2(0 + vertexCount, 1 + vertexCount));
		vertexCount += 2;
		// X Negative Lines.
		vertices.emplace_back(VertexData({ -currentPosition, maxVertexCoord, -1.f }, m_coarseGridColor, 0));
		vertices.emplace_back(VertexData({ -currentPosition, -maxVertexCoord, -1.f }, m_coarseGridColor, 0));
		indices.emplace_back(IndexData2(0 + vertexCount, 1 + vertexCount));
		vertexCount += 2;
		// Y Positive Lines.
		vertices.emplace_back(VertexData({ maxVertexCoord, currentPosition, -1.f }, m_coarseGridColor, 0));
		vertices.emplace_back(VertexData({ -maxVertexCoord, currentPosition, -1.f }, m_coarseGridColor, 0));
		indices.emplace_back(IndexData2(0 + vertexCount, 1 + vertexCount));
		vertexCount += 2;
		// Y Negative Lines.
		vertices.emplace_back(VertexData({ maxVertexCoord, -currentPosition, -1.f }, m_coarseGridColor, 0));
		vertices.emplace_back(VertexData({ -maxVertexCoord, -currentPosition, -1.f }, m_coarseGridColor, 0));
		indices.emplace_back(IndexData2(0 + vertexCount, 1 + vertexCount));
		vertexCount += 2;
		currentPosition += m_coarseIncrementSize;
	}
	m_coarseVAO->push(vertices);
	m_coarseVAO->push(indices);
	vertices.clear();
	indices.clear();

	// --------------------------- //
	//  F I N E   V E R T I C E S  //
	// --------------------------- //

	currentPosition = m_fineIncrementSize;
	vertexCount = 0;
	while (currentPosition <= maxVertexCoord)
	{
		// X Positive Lines.
		vertices.emplace_back(VertexData({ currentPosition, maxVertexCoord, -1.f }, m_fineGridColor, 0));
		vertices.emplace_back(VertexData({ currentPosition, -maxVertexCoord, -1.f }, m_fineGridColor, 0));
		indices.emplace_back(IndexData2(0 + vertexCount, 1 + vertexCount));
		vertexCount += 2;
		// X Negative Lines.
		vertices.emplace_back(VertexData({ -currentPosition, maxVertexCoord, -1.f }, m_fineGridColor, 0));
		vertices.emplace_back(VertexData({ -currentPosition, -maxVertexCoord, -1.f }, m_fineGridColor, 0));
		indices.emplace_back(IndexData2(0 + vertexCount, 1 + vertexCount));
		vertexCount += 2;
		// Y Positive Lines.
		vertices.emplace_back(VertexData({ maxVertexCoord, currentPosition, -1.f }, m_fineGridColor, 0));
		vertices.emplace_back(VertexData({ -maxVertexCoord, currentPosition, -1.f }, m_fineGridColor, 0));
		indices.emplace_back(IndexData2(0 + vertexCount, 1 + vertexCount));
		vertexCount += 2;
		// Y Negative Lines.
		vertices.emplace_back(VertexData({ maxVertexCoord, -currentPosition, -1.f }, m_fineGridColor, 0));
		vertices.emplace_back(VertexData({ -maxVertexCoord, -currentPosition, -1.f }, m_fineGridColor, 0));
		indices.emplace_back(IndexData2(0 + vertexCount, 1 + vertexCount));
		vertexCount += 2;
		currentPosition += m_fineIncrementSize;
	}
	m_fineVAO->push(vertices);
	m_fineVAO->push(indices);

	return *this;
}

Grid& Grid::destroyGrid() 
{
	disableHelperCircle();
	/*m_coarseVAO->wipeAll();
	m_fineVAO->wipeAll();
	m_originVAO->wipeAll();*/
	return *this;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//