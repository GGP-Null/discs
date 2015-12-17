#include "DiscTrail.h"
#include "Disc.h"


DiscTrail::DiscTrail(Mesh *mesh, Material *mat, Disc *disc)
	: attachedTo(disc), GameObject(mesh, mat)
{
	Rotate(XMFLOAT3(90.0f, 0, 0));
}

float DiscTrail::distToReset = 1.0f;

void DiscTrail::Update(FrameUpdateData frameData)
{
	// get disc velocity
	auto discTrans = attachedTo->GetTranslation();
	XMVECTOR discPos = XMLoadFloat3(&discTrans);
	XMVECTOR deltaTrans = XMLoadFloat3(&attachedTo->velocity);
	// make self's vel 75% of it
	deltaTrans *= frameData.DeltaTime * 0.75f;
	// move
	XMVECTOR transVec = XMLoadFloat3(&translation);
	XMVECTOR newPos = XMVectorAdd(deltaTrans, transVec);
	// if dist > some magic value, reset
	XMVECTOR distVec = XMVector3Length(newPos - discPos);
	float dist;
	XMStoreFloat(&dist, distVec);

	if (dist > distToReset) newPos = discPos;

	XMStoreFloat3(&translation, newPos);
}