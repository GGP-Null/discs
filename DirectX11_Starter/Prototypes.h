#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Disc.h"
#include "DiscTrail.h"
#include "Material.h"
#include "Mesh.h"

namespace Prototypes
{
	void SetPlayerMaterial(unsigned short playerNum, Material *mat);
	void SetPlayerMesh(unsigned short playerNum, Mesh *mesh);
	Player *MakePlayer(unsigned short playerNum);

	void SetDiscMesh(Mesh *mesh);
	void SetDiscMaterial(Material *mat);
	Disc *MakeDisc(Player *player);

	void SetSquareMesh(Mesh *mesh);
	void SetSquareMaterial(Material *mat);
	GameObject *MakeSquare();

	void SetDiscTrailMesh(Mesh *mesh);
	void SetDiscTrailMaterial(Material *mat);
	DiscTrail *MakeDiscTrail(Disc *);

	void SetArenaMesh(Mesh *mesh);
	void SetArenaMaterial(Material *mat);
	GameObject *MakeArena();

	void SetPlatformMesh(Mesh *mesh);
	void SetPlatformMaterial(unsigned short playerNum, Material *mat);
	GameObject *MakePlatform(unsigned short playerNum);
};