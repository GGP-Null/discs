#include "Disc.h"
#include <math.h>
#include <stdio.h>

#include "Player.h"

#include "MeshManager.h"


Disc::Disc(Mesh* m, Material* mat, Player* p)
	: GameObject(m, mat), colliderComp(this, MeshManager::GetColliderForMesh(m))
{
	translation = XMFLOAT3(0, 0, -5);
	isActive = false;
	SetScale(XMFLOAT3(0.5f, 0.2f, 0.5f));
	player = p;
}


Disc::~Disc()
{
}

void Disc::MoveDisc(float dt)
{
	Translate(XMFLOAT3(velocity.x * dt, velocity.y * dt, velocity.z * dt));
	if (translation.z < 0)
	{
		translation = XMFLOAT3(0, 0, -5);
		isActive = false;
	}
	if (translation.z > 13.5)
	{
		translation.z = 13.5;
		velocity.z *= -1;
	}
	if (translation.x < -3.5)
	{
		translation.x = -3.5;
		velocity.x *= -1;
	}
	if (translation.x > 3.5)
	{
		translation.x = 3.5;
		velocity.x *= -1;
	}
	if (translation.z < 6.75 && velocity.z < 0)
	{
		float numthing = 0 - translation.z / velocity.z;
		//float xdest = numthing * velocity.x + translation.x;
		float goalxvel = (player->GetTranslation().x - translation.x) / numthing;
		velocity.x += (goalxvel - velocity.x) * dt * 5;
	}
}
void Disc::Launch(XMFLOAT3 p, XMFLOAT3 r)
{
	isActive = true;
	SetTranslation(p);
	SetRotation(r);
	velocity = XMFLOAT3(10 * sin(rotation.y), 0, 10 * cos(rotation.y));
	//velocity = XMFLOAT3(5, 0, 10 * cos(rotation.y));
}