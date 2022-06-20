//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Grid.h"
#include "OpenGL/Buffers/GraphicsPrimitivesBuffersGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Primitives/Circle.h"
#include "OpenGL/Renderer/RendererGL.h"

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

Grid::Grid() 
{
	// Add the circle used to display the nearest vertex.
	m_helperCircle = Renderer::addCircle2D({ 0.f, 0.f, -0.99999 }, m_fineIncrementSize / 5.f, m_helperCircleColor);
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
	float z = -0.99;
	// Setup the Buffer's.
	m_fineBuffer.setCapacityIncrements(totalFineVerts);
	m_fineBuffer.setCapacityIncrements(totalFineVerts);
	m_coarseBuffer.setCapacityIncrements(totalCoarseVerts);
	m_coarseBuffer.setCapacityIncrements(totalCoarseVerts);
	m_originBuffer.setCapacityIncrements(4);
	std::vector<VertexData> vertices;
	vertices.reserve(totalFineVerts);
	std::vector<UInt2> indices;

	// ------------- //
	//  O R I G I N  //
	// ------------- //

	vertices.emplace_back(glm::vec3{ 0.f, maxVertexCoord, z }, m_xAxisColor, 0);
	vertices.emplace_back(glm::vec3{ 0.f, -maxVertexCoord, z }, m_xAxisColor, 0);
	vertices.emplace_back(glm::vec3{ maxVertexCoord, 0.f, z }, m_yAxisColor, 0);
	vertices.emplace_back(glm::vec3{ -maxVertexCoord, 0.f, z }, m_yAxisColor, 0);
	indices.emplace_back(0, 1);
	indices.emplace_back(2, 3);
	m_originBuffer.push(vertices.data(), vertices.size(), indices.data(), indices.size());
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
		vertices.emplace_back(glm::vec3{ currentPosition, maxVertexCoord, z }, m_coarseGridColor, 0);
		vertices.emplace_back(glm::vec3{ currentPosition, -maxVertexCoord, z }, m_coarseGridColor, 0);
		indices.emplace_back(0 + vertexCount, 1 + vertexCount);
		vertexCount += 2;
		// X Negative Lines.
		vertices.emplace_back(glm::vec3{ -currentPosition, maxVertexCoord, z }, m_coarseGridColor, 0);
		vertices.emplace_back(glm::vec3{ -currentPosition, -maxVertexCoord, z }, m_coarseGridColor, 0);
		indices.emplace_back(0 + vertexCount, 1 + vertexCount);
		vertexCount += 2;
		// Y Positive Lines.
		vertices.emplace_back(glm::vec3{ maxVertexCoord, currentPosition, z }, m_coarseGridColor, 0);
		vertices.emplace_back(glm::vec3{ -maxVertexCoord, currentPosition, z }, m_coarseGridColor, 0);
		indices.emplace_back(0 + vertexCount, 1 + vertexCount);
		vertexCount += 2;
		// Y Negative Lines.
		vertices.emplace_back(glm::vec3{ maxVertexCoord, -currentPosition, z }, m_coarseGridColor, 0);
		vertices.emplace_back(glm::vec3{ -maxVertexCoord, -currentPosition, z }, m_coarseGridColor, 0);
		indices.emplace_back(0 + vertexCount, 1 + vertexCount);
		vertexCount += 2;
		currentPosition += m_coarseIncrementSize;
	}
	m_coarseBuffer.push(vertices.data(), vertices.size(), indices.data(), indices.size());
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
		vertices.emplace_back(glm::vec3{ currentPosition, maxVertexCoord, z }, m_fineGridColor, 0);
		vertices.emplace_back(glm::vec3{ currentPosition, -maxVertexCoord, z }, m_fineGridColor, 0);
		indices.emplace_back(0 + vertexCount, 1 + vertexCount);
		vertexCount += 2;
		// X Negative Lines.
		vertices.emplace_back(glm::vec3{ -currentPosition, maxVertexCoord, z }, m_fineGridColor, 0);
		vertices.emplace_back(glm::vec3{ -currentPosition, -maxVertexCoord, z }, m_fineGridColor, 0);
		indices.emplace_back(0 + vertexCount, 1 + vertexCount);
		vertexCount += 2;
		// Y Positive Lines.
		vertices.emplace_back(glm::vec3{ maxVertexCoord, currentPosition, z }, m_fineGridColor, 0);
		vertices.emplace_back(glm::vec3{ -maxVertexCoord, currentPosition, z }, m_fineGridColor, 0);
		indices.emplace_back(0 + vertexCount, 1 + vertexCount);
		vertexCount += 2;
		// Y Negative Lines.
		vertices.emplace_back(glm::vec3{ maxVertexCoord, -currentPosition, z }, m_fineGridColor, 0);
		vertices.emplace_back(glm::vec3{ -maxVertexCoord, -currentPosition, z }, m_fineGridColor, 0);
		indices.emplace_back(0 + vertexCount, 1 + vertexCount);
		vertexCount += 2;
		currentPosition += m_fineIncrementSize;
	}

	m_fineBuffer.push(vertices.data(), vertices.size(), indices.data(), indices.size());
	return *this;
}

Grid& Grid::destroyGrid() 
{
	disableHelperCircle();
	/*m_coarseBuffer.;
	m_fineBuffer.wipeAll();
	m_originBuffer.wipeAll();*/
	return *this;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//