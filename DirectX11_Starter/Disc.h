#pragma once
#include "GameObject.h"
#include "CylinderColliderComponent.h"

class Player;
class DiscTrail;

class Disc : public GameObject
{
public:
	Disc(Mesh* m, Material* mat, Player* p);
	~Disc();
	void Launch(XMFLOAT3 p, XMFLOAT3 r);
	void MoveDisc(float dt);
	bool IsActive() const { return isActive; }
	void OnHit();
	CylinderColliderComponent colliderComp;
	XMFLOAT3 velocity;
	DiscTrail *trail;
private:
	bool isActive;
	Player* player;
};