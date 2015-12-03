#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "GameObject.h"

using namespace DirectX;

class TrackingCamera : public Camera
{
private:
	XMFLOAT3 forward;
	GameObject* target;
	XMFLOAT3 up;
	float trackStrength;

public:
	TrackingCamera(XMFLOAT3 position, XMFLOAT3 forward, GameObject* target, XMFLOAT3 up, float trackStrength, float aspectRatio);
	~TrackingCamera();

	void SetPosition(XMFLOAT3 position);
	void SetForward(XMFLOAT3 forward);
	void BindNewTarget(GameObject* target);
	void SetTrackingStrength(float trackStrength);

	virtual void Update(float deltaTime, float totalTime) override;
};

