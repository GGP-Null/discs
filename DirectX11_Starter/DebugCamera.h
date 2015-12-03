#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include "Camera.h"
#include "SimpleShader.h"

using namespace DirectX;

/// <summary>
/// Basic camera.
/// </summary>
class DebugCamera : public Camera
{
private:
	bool viewMatIsDirty;

	XMFLOAT3 direction;

	float rotX;
	float rotY;

public:
	DebugCamera(XMFLOAT3 position, XMFLOAT3 direction, float aspectRatio);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetDirection();
	float GetXRotation();
	float GetYRotation();


	void SetPosition(XMFLOAT3 position);
	void SetDirection(XMFLOAT3 direction);
	void SetRotationX(float rotX);
	void SetRotationY(float rotY);
	void Rotate(float x, float y);

	virtual void Update(float deltaTime, float totalTime) override;

	~DebugCamera();
};

