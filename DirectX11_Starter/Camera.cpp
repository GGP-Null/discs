#include "Camera.h"



Camera::Camera(float ar)
{
	this->CreateProjMatrix(ar);
}

Camera::~Camera()
{
}

/// <summary>
/// Builds the camera's left handed perspective matrix.  Call this
/// method whenever the size of the window changes.
/// 
/// Uses a FOV angle of 45 degrees (pi/4 rad) and the given aspect ratio.
/// Near and far clipping plane are at 0.1 and 100, respectively.
/// </summary>
/// <param name="aspectRatio">the aspect ratio of the window</param>
void Camera::CreateProjMatrix(float aspectRatio)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		aspectRatio,		  	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projMat, XMMatrixTranspose(P)); // Transpose for HLSL!
}

/// <summary>
/// Sets the camera's view and perspective matrices in a shader,
/// assuming that the matrices in the shader are named "view" and
/// "projection" respectively.
/// 
/// This does NOT send buffer data.
/// </summary>
/// <param name="vertexShader">the vertex shader to set the matrices in</param>
void Camera::SetViewAndProjMatrices(SimpleVertexShader* vertexShader)
{
	vertexShader->SetMatrix4x4("view", viewMat);
	vertexShader->SetMatrix4x4("projection", projMat);
}

XMFLOAT3 Camera::GetPosition() const
{
	return position;
}

XMFLOAT4X4 Camera::getView()
{
	return viewMat;
}

XMFLOAT4X4 Camera::getProjection()
{
	return projMat;
}
