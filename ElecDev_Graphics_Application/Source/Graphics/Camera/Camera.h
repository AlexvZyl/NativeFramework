#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "glm/glm.hpp"

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
	// Resize the camera by size.
	void resize(const glm::vec2& size);
	// Resize the camera by viewport.
	void resize(const glm::vec4& viewport);
	// Calculate the world coordinates from the pixel coordinates.
	glm::vec3 pixelToWorldCoords(const glm::vec2& pixelCoords, bool useUpdatedView = false);
	// Calculate the pixel coordinates from the world coordinates.
	glm::vec2 worldToPixelCoords(const glm::vec3& worldCoords, bool useUpdatedView = false);

	// Getters.
	const glm::vec4& getViewport() const;
	const glm::vec2& getViewportSize() const;
	const CameraType& getType() const;
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
	// Manually scale the camera in 2 dimensions.
	void scale2D(float scale);
	// Manually scale the camera in all dimensions.
	void scale(const glm::vec3& scale);
	// Manually scale the camera, around the provided position.
	void scaleAroundCursor(float scale, const glm::vec2& cursor);
	// Inrement the zoom, based on the scale rate set.
	void incrementZoomLevel(int increment);
	// Inrement the zoom, based on the scale rate set, around the provided position.
	void incrementZoomAroundCursor(int increment, const glm::vec2& cursor);
	// Make the camera look at a point.
	void lookAt(const glm::vec3& position) {}

private:


	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	void setViewport(const glm::vec2& viewport);
	void setViewport(const glm::vec4& viewport);
	void viewChanged();
	void projectionChanged();
	void updateViewMatrix();
	void updateProjectionMatrix();
	void updateViewProjectionMatrix();
	void updateAllMatrices();
	float getScaleFromIncrement(int increment);

	// --------- //
	//  D A T A  //
	// --------- //

	glm::mat4 m_viewMatrix			   = glm::mat4(1.0f);
	glm::mat4 m_prevViewMatrix		   = glm::mat4(1.0f);
	glm::mat4 m_projectionMatrix	   = glm::mat4(1.0f);			
	glm::mat4 m_viewProjectionMatrix   = glm::mat4(1.0f);
	glm::mat4 m_scalingMatrix		   = glm::mat4(1.0f);			
	glm::vec3 m_position			   = glm::vec3(0.f);
	glm::mat4 m_rotationMatrix		   = glm::mat4(1.0f);			
	glm::vec4 m_viewport			   = glm::vec4(1.0f);					
	float m_scaleRate				   = 1.f;			
	bool m_viewMatrixChanged		   = true;
	bool m_projectionMatrixChanged	   = true;
	bool m_viewProjectionMatrixChanged = true;
	CameraType m_type;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
