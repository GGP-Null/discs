#include "Disc.h"
#include <math.h>


Disc::Disc(Mesh* m, Material mat)
	: GameObject(m, mat)
{
	translation = XMFLOAT3(0, 0, -5);
	isActive = false;
}


Disc::~Disc()
{
}

void Disc::MoveDisc(float dt)
{
	Translate(XMFLOAT3(velocity.x * dt, velocity.y * dt, velocity.z * dt));
	if (translation.z >= 20)
	{
		translation = XMFLOAT3(0, 0, -5);
		isActive = false;
	}
}
void Disc::Launch(XMFLOAT3 p, XMFLOAT3 r)
{
	isActive = true;
	SetTranslation(p);
	SetRotation(r);
	velocity = XMFLOAT3(4*sin(rotation.y), 0, 4 * cos(rotation.y));
}
