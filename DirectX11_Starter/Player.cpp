#include "Player.h"
#include "Disc.h"
using namespace Input;

Player::Player(Mesh* m, Material* mat)
	: Program(m, mat)
{
	ableToFire = true;
}


Player::~Player()
{}

void Player::Fire(Disc* d)
{
	if (ableToFire)
	{
		d->Launch(translation, rotation);
		ableToFire = false;
	}
}

void Player::ReloadDisc()
{
	ableToFire = true;
}

void Player::SetNum(int num)
{
	this->playerNum = num;
}

int Player::GetNum()
{
	return playerNum;
}

void Player::Update(FrameUpdateData upData)
{
	auto deltaTime = upData.DeltaTime;
	auto &padState = gamePad.GetState();

	if (KeyIsDown(Keys::J)) Translate(XMFLOAT3(-deltaTime, 0, 0));
	else if (KeyIsDown(Keys::K)) Translate(XMFLOAT3(deltaTime, 0, 0));
	if (KeyIsDown(Keys::U)) Rotate(XMFLOAT3(0, -deltaTime, 0));
	else if (KeyIsDown(Keys::I)) Rotate(XMFLOAT3(0, deltaTime, 0));

	if (padState.IsConnected()) {
		Translate(XMFLOAT3(padState.thumbSticks.leftX * deltaTime, 0, 0));
		float rotAmt = padState.triggers.right - padState.triggers.left;
		Rotate(XMFLOAT3(0, rotAmt * deltaTime, 0));
	}
}