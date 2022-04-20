#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "glm/glm.hpp"
#include <array>

//==============================================================================================================================================//
//  Camera Types.																																//
//==============================================================================================================================================//

enum class CameraType
{
	Standard2D, Standard3D
};

//==============================================================================================================================================//
//  Camera Class.																																//
//==============================================================================================================================================//

class Camera 
{
public:

	// Constructor.
	Camera(CameraType cameraType, const glm::vec2& size);
	// Updates the camera components.
	void onUpdate();
	// Set the size of the viewport (assuming a start at 0,0).
	void setViewport(const glm::vec2& viewport);
	// Set the size of the viewport.
	void setViewport(const glm::vec4& viewport);
	// Get the viewport.
	const glm::vec4& getViewport() const;
	// Get the viewport size.
	const glm::vec2& getViewportSize() const;
	// Resize the camera (viewport).
	void resize(const glm::vec2& size);

	// Calculate the world coordinates from the pixel coordinates.
	glm::vec3 pixelCoordsToWorldCoords(const glm::vec2& pixelCoords);

	// Get the type of camera.
	const CameraType& getType() const;

	// Get matrices.
	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;
	const glm::mat4& getViewProjectionMatrix() const;

	// ----------------- //
	//  C O N T R O L S  //
	// ----------------- //

	// Set the position of the camera.
	void setPosition(const glm::vec3& position);
	// Set the rate by which the camera should scale.
	void setScaleRate(float rate);
	// Translate the camera by the given vector.
	void translate(const glm::vec3& translation);
	void translate(const glm::vec2& translation);
	// Manually scale the camera.
	void scale2D(float scale);
	// Manually scale the camera, around the provided position.
	void scaleAroundCursor2D(float scale, const glm::vec2& cursor);
	// Inrement the zoom, based on the scale rate set.
	void incrementZoomLevel2D(int increment);
	// Inrement the zoom, based on the scale rate set, around the provided position.
	void incrementZoomAroundCursor2D(int increment, const glm::vec2& cursor);
	// Make the camera look at a point.
	void lookAt(const glm::vec3& position) {}

private:

	CameraType m_type;

	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	void construct2DCamera(const glm::vec2& size);
	void construct3DCamera(const glm::vec2& size);
	void viewChanged();
	void projectionChanged();
	void updateViewMatrix();
	void updateProjectionMatrix();
	void updateViewProjectionMatrix();
	void updateAllMatrices();

	// ----------------- //
	//  M A T R I C E S  //
	// ----------------- //

	// MVP Matrices.
	glm::mat4 m_viewMatrix			  = glm::mat4(1.0f);				
	glm::mat4 m_projectionMatrix	  = glm::mat4(1.0f);			
	glm::mat4 m_viewProjectionMatrix  = glm::mat4(1.0f);
	glm::mat4 m_scalingMatrix		  = glm::mat4(1.0f);			
	glm::vec3 m_position			  = glm::vec3(0.f);
	glm::mat4 m_rotationMatrix		  = glm::mat4(1.0f);			
	glm::vec4 m_viewport			  = glm::vec4(1.0f);					
	std::array<float, 6> m_projectionValues;				
	float m_scaleRate = 1.f;			
	float m_zoomInRate = 1.f;
	float m_zoomOutRate = 1.f;
	bool m_viewMatrixChanged		  = false;
	bool m_projectionMatrixChanged	  = false;
	bool m_viewProjectionMatrixChanged = false;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
