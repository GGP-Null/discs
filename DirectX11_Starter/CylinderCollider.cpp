#include "CylinderColliderBuilder.h"
#include "CylinderCollider.h"

using namespace DirectX;

CylinderCollider::CylinderCollider(XMFLOAT3 centroid, float half_height, float radius)
	: Centroid(centroid), HalfHeight(half_height), Radius(radius)
{
}

CylinderCollider CylinderCollider::Make(std::vector<XMFLOAT3> &vect)
{
	if (vect.begin() == vect.end()) return CylinderCollider();

	CylinderColliderBuilder builder(vect[0]);

	for (auto i = (vect.begin() + 1); i != vect.end(); ++i)
	{
		builder.NewPoint(*i);
	}

	return builder.Finalize();
}

CylinderCollider CylinderCollider::Make(std::vector<Vertex> &vect)
{
	if (vect.begin() == vect.end()) return CylinderCollider();

	CylinderColliderBuilder builder(vect[0].Position);

	for (auto i = (vect.begin() + 1); i != vect.end(); ++i)
	{
		builder.NewPoint(i->Position);
	}

	return builder.Finalize();
}

bool IsColliding(CylinderCollider const &cyl_1, CylinderCollider const &cyl_2)
{
	// TODO: rotation

	float collision_length = cyl_1.Radius + cyl_2.Radius;

	bool y_colliding =
		(cyl_1.Centroid.y + cyl_1.HalfHeight > cyl_2.Centroid.y - cyl_2.HalfHeight) ||
		(cyl_2.Centroid.y + cyl_2.HalfHeight > cyl_1.Centroid.y - cyl_1.HalfHeight);

	XMVECTOR cent_1 = XMLoadFloat3(&cyl_1.Centroid);
	XMVECTOR cent_2 = XMLoadFloat3(&cyl_2.Centroid);
	XMVECTOR cent_1_2d = XMVectorSetY(cent_1, 0);
	XMVECTOR cent_2_2d = XMVectorSetY(cent_2, 0);

	float dist = XMVectorGetX(XMVector3Length(cent_2_2d - cent_1_2d));

	bool xz_colliding = dist <= collision_length;

	return y_colliding && xz_colliding;
}