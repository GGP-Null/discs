#pragma once

#include <DirectXMath.h>
#include <vector>
#include "Vertex.h"

using DirectX::XMFLOAT3;

class CylinderCollider
{
public:
	CylinderCollider() = default;
	CylinderCollider(XMFLOAT3 centroid, float half_height, float radius);

	static CylinderCollider Make(std::vector<XMFLOAT3> &vect);
	static CylinderCollider Make(std::vector<Vertex> &vect);

	XMFLOAT3 Centroid;
	float HalfHeight, Radius;
};

bool IsColliding(CylinderCollider const &cyl1, CylinderCollider const &cyl2);