#include "Prototypes.h"
#include <array>
#include <sstream>
#include "Input.h"

using namespace Input;

std::array<Mesh *, 2> playerMeshes;
Mesh *discMesh, *arenaMesh, *platformMesh;

std::array<Material *, 2> platformMaterial;
std::array<Material *, 2> playerMaterials;
Material *discMaterial, *arenaMaterial;

void Prototypes::SetPlayerMaterial(unsigned short playerNum, Material *mat)
{
	playerMaterials[playerNum] = mat;
}

void Prototypes::SetPlayerMesh(unsigned short playerNum, Mesh *mesh)
{
	playerMeshes[playerNum] = mesh;
}

Player *Prototypes::MakePlayer(unsigned short playerNum)
{
	Player* player;
	switch (playerNum)
	{
	default:
	case 0:
		player = new Player(playerMeshes[playerNum], playerMaterials[playerNum], Keys::J, Keys::L, Keys::O, Keys::U, Keys::I);
		break;
	case 1:
		player = new Player(playerMeshes[playerNum], playerMaterials[playerNum], Keys::A, Keys::D, Keys::Q, Keys::E, Keys::W);
		break;
	}

	player->Scale(XMFLOAT3(.1f, .1f, .1f));
	player->Rotate(XMFLOAT3(0.0f, -3.1415 / 2.0, 0.0f));

	if (playerNum == 1) player->Translate(XMFLOAT3(0, 0.0, 12.0));

	//players can know their player number, so why not set it here?
	player->SetNum(playerNum);

	stringstream playerName("player");
	playerName << playerNum + 1;

	player->SetDebugName(playerName.str());

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
	auto disc = new Disc(discMesh, discMaterial, player);

	disc->SetDebugName(player->GetDebugName() + "_disc");

	return disc;
}

void Prototypes::SetArenaMesh(Mesh *mesh)
{
	arenaMesh = mesh;
}

void Prototypes::SetArenaMaterial(Material *mat)
{
	arenaMaterial = mat;
}

GameObject *Prototypes::MakeArena()
{
	auto arena = new GameObject(arenaMesh, arenaMaterial);
	arena->SetScale(XMFLOAT3(7, 7, 17));
	arena->SetTranslation(XMFLOAT3(0, 0, 6));

	arena->SetDebugName("arena");
	return arena;
}

void Prototypes::SetPlatformMesh(Mesh *mesh)
{
	platformMesh = mesh;
}

void Prototypes::SetPlatformMaterial(unsigned short playerNum, Material *mat)
{
	platformMaterial[playerNum] = mat;
}

GameObject *Prototypes::MakePlatform(unsigned short playerNum)
{
	float zLoc;
	switch (playerNum) {
	default:
	case 0:
		zLoc = 0.0;
		break;
	case 1:
		zLoc = 12.0;
		break;
	}

	auto platform = new GameObject(platformMesh, platformMaterial[playerNum]);

	platform->SetScale(XMFLOAT3(0.5, 0.5, 0.5));
	platform->Translate(XMFLOAT3(0, -0.2, zLoc));

	stringstream platName("player");
	platName << playerNum + 1 << "_platform";

	platform->SetDebugName(platName.str());

	return platform;
}