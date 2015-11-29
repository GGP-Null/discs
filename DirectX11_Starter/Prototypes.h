#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Disc.h"
#include "Material.h"
#include "Mesh.h"

namespace Prototypes
{
	void SetPlayerMaterial(Material *mat);
	void SetPlayerMesh(unsigned short playerNum, Mesh *mesh);

	Player *MakePlayer(unsigned short playerNum);

	void SetDiscMesh(Mesh *mesh);
	void SetDiscMaterial(Material *mat);
	Disc *MakeDisc(Player *player);

	void SetArenaMesh(Mesh *mesh);
	GameObject *MakeArena();
	void SetPlatformMesh(Mesh *mesh);
	GameObject *MakePlatform(unsigned short playerNum = 0);
};