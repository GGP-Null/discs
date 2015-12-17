#pragma once
#include "GameObject.h"

class Disc;

struct DiscTrail :
	public GameObject
{
	DiscTrail(Mesh *, Material *, Disc *);
	virtual void Update(FrameUpdateData);

	static float distToReset;

	Disc *attachedTo;
};

