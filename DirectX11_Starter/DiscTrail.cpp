#include "DiscTrail.h"
#include "Disc.h"


DiscTrail::DiscTrail(Mesh *mesh, Material *mat, Disc *disc)
	: attachedTo(disc), GameObject(mesh, mat), duration(0)
{
	//Rotate(XMFLOAT3(00.0f, 90.0f, 0));
}

float DiscTrail::timeToReset = 4.0f;
float DiscTrail::maxDist = 2.5f;

static float
quicklerp(float from, float to, float t)
{
	return from + (t * (to - from));
}

void DiscTrail::Update(FrameUpdateData frameData)
{
	duration += frameData.DeltaTime;
	if (duration > timeToReset) duration = 0.0f;

	// lerp
	float dist = quicklerp(0.0f, maxDist, duration/timeToReset);

	XMVECTOR discVel = XMLoadFloat3(&attachedTo->velocity);
	XMVECTOR discDir = XMVector3Normalize(discVel);

	XMVECTOR deltaPos = XMVectorMultiply(discDir, XMVectorReplicate(dist));

	auto discPos = attachedTo->GetTranslation();
	XMVECTOR discPosVec = XMLoadFloat3(&discPos);

	XMVECTOR newPos = discPosVec - deltaPos;

	XMFLOAT3 newPosF;
	XMStoreFloat3(&newPosF, newPos);
	SetTranslation(newPosF);
}