#pragma once
#include "GameObject.h"
#include "Disc.h"

class Player: public GameObject
{
public:
	Player(Mesh* m, Material mat);
	~Player();
	void Fire(Disc* d);
	void ReloadDisc();
private:
	bool ableToFire;
};

