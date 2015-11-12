#include "CylinderColliderBuilder.h"
#include "CylinderCollider.h"
#include "Vertex.h"

using namespace DirectX;

CylinderColliderBuilder::CylinderColliderBuilder() :
	Max(0,0,0), Min(0,0,0)
{ }

CylinderColliderBuilder::CylinderColliderBuilder(XMFLOAT3 point) :
	Max(point), Min(point)
{ }

void CylinderColliderBuilder::Start(XMFLOAT3 point)
{
	Max = point;
	Min = point;
}

void CylinderColliderBuilder::NewPoint(XMFLOAT3 point)
{
	if (point.x > Max.x) Max.x = point.x;
	if (point.y > Max.y) Max.y = point.y;
	if (point.z > Max.z) Max.z = point.z;

	if (point.x < Min.x) Min.x = point.x;
	if (point.y < Min.y) Min.y = point.y;
	if (point.z < Min.z) Min.z = point.z;
}


CylinderCollider CylinderColliderBuilder::Finalize()
{
	XMVECTOR max = XMLoadFloat3(&this->Max);
	XMVECTOR min = XMLoadFloat3(&this->Min);

	XMVECTOR v_centroid = (max + min) / 2;
	XMVECTOR v_halves = max - v_centroid;

	XMFLOAT3 centroid, halves;
	XMStoreFloat3(&centroid, v_centroid);
	XMStoreFloat3(&halves, v_halves);

	XMVECTOR max_in_xz_plane = XMVectorSet(Max.x, 0, Max.z, 0);

	XMVECTOR v_length = XMVector3Length(max_in_xz_plane - v_centroid);

	float radius;
	XMStoreFloat(&radius, v_length);

	return CylinderCollider(centroid, halves.y, radius);
}