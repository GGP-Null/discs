#pragma once
#include "Program.h"

class Disc;

class Player: public Program
{
public:
	Player(Mesh* m, Material* mat);
	~Player();
	void Fire(Disc* d);
	void ReloadDisc();
private:
	bool ableToFire;
};