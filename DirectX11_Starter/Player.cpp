#include "Player.h"
#include "Disc.h"

using namespace Input;

Player::Player(Mesh* m, Material* mat, Key lKey, Key rKey, Key cwKey, Key ccwKey, Key fireKey)
	: Program(m, mat)
{
	ableToFire = true;
	isActive = true;
	numTimesHit = 0;
	moveLeft = lKey;
	moveRight = rKey;
	rotRight = cwKey;
	rotLeft = ccwKey;
	fire = fireKey;
	if (lKey == Keys::A)
	{
		gamePad = Input::GamePad::FromId(1);
	}
}


Player::~Player()
{}

void Player::Fire(Disc* d)
{
	if (ableToFire)
	{
		d->Launch(XMFLOAT3(translation.x, translation.y + 0.5f, translation.z), rotation);
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

void Player::OnDiscHit()
{
	numTimesHit++;
	if (numTimesHit >= 2) isActive = false;
}

void Player::Update(FrameUpdateData upData)
{
	auto deltaTime = upData.DeltaTime;
	auto &padState = gamePad.GetState();

	if (KeyIsDown(moveLeft)) Translate(XMFLOAT3(-deltaTime, 0, 0));
	else if (KeyIsDown(moveRight)) Translate(XMFLOAT3(deltaTime, 0, 0));
	if (KeyIsDown(rotLeft)) Rotate(XMFLOAT3(0, -deltaTime, 0));
	else if (KeyIsDown(rotRight)) Rotate(XMFLOAT3(0, deltaTime, 0));

	if (padState.IsConnected()) {
		Translate(XMFLOAT3(padState.thumbSticks.leftX * deltaTime, 0, 0));
		float rotAmt = padState.triggers.right - padState.triggers.left;
		Rotate(XMFLOAT3(0, rotAmt * deltaTime, 0));
	}

	if (playerNum == 0)
	{
		Rotate(XMFLOAT3(0, Input::GetMouseX() * deltaTime * .05f, 0));
	}
}

void Player::Draw(ID3D11DeviceContext* context) 
{
	if (isActive) GameObject::Draw(context);
}