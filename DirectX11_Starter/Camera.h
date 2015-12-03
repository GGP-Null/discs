#pragma once
#include <DirectXMath.h>
#include "SimpleShader.h"

using namespace DirectX;

class Camera
{
protected:
	XMFLOAT4X4 viewMat;
	XMFLOAT4X4 projMat;
	XMFLOAT3 position;

public:
	Camera(float aspectRatio);
	~Camera();

	virtual void CreateProjMatrix(float aspectRatio);
	void SetViewAndProjMatrices(SimpleVertexShader* vertexShader);
	virtual void Update(float deltaTime, float totalTime) {};

	XMFLOAT3 GetPosition() const;

	XMFLOAT4X4 getView();
	XMFLOAT4X4 getProjection();
};

