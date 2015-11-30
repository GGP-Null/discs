#include "Prototypes.h"
#include <array>

std::array<Mesh *, 2> playerMeshes;
Mesh *discMesh, *arenaMesh, *platformMesh;

std::array<Material *, 2> platformMaterial;
Material *playerMaterial, *discMaterial, *arenaMaterial;

void Prototypes::SetPlayerMaterial(Material *mat)
{
	playerMaterial = mat;
}

void Prototypes::SetPlayerMesh(unsigned short playerNum, Mesh *mesh)
{
	playerMeshes[playerNum] = mesh;
}

Player *Prototypes::MakePlayer(unsigned short playerNum)
{
	auto player = new Player(playerMeshes[playerNum], playerMaterial);

	player->Scale(XMFLOAT3(.1f, .1f, .1f));

	if (playerNum == 1) player->Translate(XMFLOAT3(0, 0.0, 12.0));

	//players can know their player number, so why not set it here?
	player->SetNum(playerNum);

	return player;
}

void Prototypes::SetDiscMaterial(Material *mat)
{
	discMaterial = mat;
}

void Prototypes::SetDiscMesh(Mesh *mesh)
{
	discMesh = mesh;
}

Disc *Prototypes::MakeDisc(Player *player)
{
	return new Disc(discMesh, discMaterial, player);
}

void SetArenaMesh(Mesh *mesh)
{

}