#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "GameObject.h"
#include "Material.h"
#include "FrameUpdateData.h"

class ParticleEmitter
{
public:
	virtual void Update(FrameUpdateData);
	virtual void Draw(ID3D11DeviceContext *context);
	void SetOffset(XMFLOAT3 offset);
	void SetParent(GameObject *parent);
private:
	Material *mat;
	DirectX::XMFLOAT3 offset;
	GameObject *parent;
};