/*
This code contains the class that handles the mouse point that snaps to components on the screen.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "mousePoint.h"
#include "../../Resources/resource.h"

const float PI = 3.14159265358979323f;

//----------------------------------------------------------------------------------------------------------------------
//  Mouse methods.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
MousePoint::MousePoint(float color[4], float radius, unsigned int resolution, glm::mat4* projectionMatrix, stateMachine* states)
	: m_projectionMatrix(projectionMatrix), m_states(states)
{
	// Create VAO that draws the point.
	m_VAO = new VertexArrayObject(GL_TRIANGLES);

	// Create shader to render the point.
	m_shader = new Shader(BASIC_SHADER);

	// Assign matrices to shader.
	m_shader->bind();
	m_shader->setMat4("worldMatrix", m_modelMatrix);
	m_shader->setMat4("projectionMatrix", *m_projectionMatrix);
	m_shader->setMat4("viewMatrix", m_viewMatrix);

	// Lines used to draw the circle.
	std::vector<VertexData> vertices;

	// Calculate coordinates to draw to.
	float coords[2] = { 0,0 };

	// Create triangles requried to draw a circle.
	for (int i = 0; i <= resolution; i++)
	{
		float x1 = coords[0] + radius * std::cos((i - 1) * PI * 2 / resolution);
		float y1 = coords[1] + radius * std::sin((i - 1) * PI * 2 / resolution);
		float x2 = coords[0] + radius * std::cos(i * PI * 2 / resolution);
		float y2 = coords[1] + radius * std::sin(i * PI * 2 / resolution);
		VertexData v1(coords[0], coords[1], 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v2(x1, y1, 0.0f, color[0], color[1], color[2], color[3]);
		VertexData v3(x2, y2, 0.0f, color[0], color[1], color[2], color[3]);
		std::vector<VertexData> verticesTemp = { v1, v2, v3 };
		// Add triangle to total triangles.
		vertices.insert(vertices.end(), verticesTemp.begin(), verticesTemp.end());
	}
	// Write the data to the buffer.
	m_VAO->writeData(vertices);
}

// Destructor.
MousePoint::~MousePoint() 
{
	delete m_VAO;
	delete m_shader;
}

void MousePoint::render()
{
	// Assign shader changes and render.
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	m_shader->bind();
	m_shader->setMat4("viewMatrix", m_viewMatrix);
	m_VAO->render();
}

// Update the projection matrix of the mouse point shader.
void MousePoint::updateProjection() 
{
	m_shader->bind();
	m_shader->setMat4("projectionMatrix", *m_projectionMatrix);
}

// Update the mouse point position based on the cursor position.
void MousePoint::updatePosition(float pixelCoords[2])
{
	// Calculate the distance it has to travel in world coords and assign to the shader matrix.
	glm::vec4 newWorldCoords = pixelCoordsToWorldCoords(pixelCoords);
	float translate[2] = { m_worldCoordsPrev[0] - newWorldCoords[0], m_worldCoordsPrev[1] - newWorldCoords[1] };
	m_translationMatrix =  glm::translate(m_translationMatrix, glm::vec3(-translate[0], -translate[1], 0.0f));

	// Assign current position to prev coords.
	m_worldCoordsPrev[0] = newWorldCoords[0];
	m_worldCoordsPrev[1] = newWorldCoords[1];
}

// Function that takes pixel coordinates as input and return the coordinates in the world.
glm::vec4 MousePoint::pixelCoordsToWorldCoords(float pixelCoords[2])
{
	// The coordinates on the screen.
	float screenCoords[2];
	// Find the viewpwort dimensions.
	float viewport[2] = { m_states->renderWindowSize.x, m_states->renderWindowSize.y };
	// Account for pixel offset.
	float viewportOffset[2] = { (float)viewport[0], (float)viewport[1] };
	// OpenGL places the (0,0) point in the top left of the screen.  Place it in the bottom left cornder.
	float pixelCoordsTemp[2] = { pixelCoords[0] , (float)viewport[1] - pixelCoords[1] };

	// Apply the viewport transform the the pixels.
	screenCoords[0] = (pixelCoordsTemp[0] - viewportOffset[0] / 2) / (viewportOffset[0] / 2);
	screenCoords[1] = (pixelCoordsTemp[1] - viewportOffset[1] / 2) / (viewportOffset[1] / 2);
	// Convert to screen vector.
	glm::vec4 screenVec = { screenCoords[0], screenCoords[1], 0, 1 };

	// Apply MVP matrices.
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	glm::mat4 MVPinverse = glm::inverse(m_modelMatrix * m_viewMatrix * *m_projectionMatrix);
	glm::vec4 worldVec = screenVec * MVPinverse;

	return worldVec;
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
