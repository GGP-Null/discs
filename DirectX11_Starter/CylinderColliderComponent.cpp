#define NOMINMAX
// #whatsastandard
#include "CylinderColliderComponent.h"
#include <DirectXMath.h>
#include <algorithm>
using namespace DirectX;

CylinderColliderComponent::CylinderColliderComponent(GameObject *go, CylinderCollider *col)
	:go(go), collider(col)
{
}

void CylinderColliderComponent::SetCollider(CylinderCollider *col)
{
	collider = col;
}

CylinderCollider *CylinderColliderComponent::GetCollider()
{
	return collider;
}

void CylinderColliderComponent::SetGameObject(GameObject *go)
{
	this->go = go;
}

GameObject *CylinderColliderComponent::GetGameObject()
{
	return go;
}

/*
XMFLOAT4X4 GetNonRotatedWorldMatrix()
{
	//create matrices from transformations
	XMMATRIX mTrans = XMMatrixTranslationFromVector(XMLoadFloat3(&translation));
	XMMATRIX mScale = XMMatrixScalingFromVector(XMLoadFloat3(&scale));

	//combine transformations
	XMMATRIX mWorldMat = mScale * mTrans;

	XMFLOAT4X4 nonRotatedWorldMatrix;
	XMStoreFloat4x4(&nonRotatedWorldMatrix, XMMatrixTranspose(mWorldMat));

	return nonRotatedWorldMatrix;
}
*/

CylinderColliderComponent::operator CylinderCollider()
{
	XMFLOAT3 goScale = go->GetScale();
	float maxXZscale = std::max(goScale.x, goScale.z);

	float newHalfHeight = collider->HalfHeight * goScale.y;
	float newRadius = collider->Radius * maxXZscale;

	XMFLOAT3 goTranslation = go->GetTranslation();
	XMFLOAT3 colCentroid = collider->Centroid;

	XMFLOAT3 newCentroid(
		goTranslation.x + colCentroid.x,
		goTranslation.y + colCentroid.y,
		goTranslation.z + colCentroid.z
		);

	return CylinderCollider(newCentroid, newHalfHeight, newRadius);
}