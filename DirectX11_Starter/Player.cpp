#include "Player.h"
#include "Disc.h"
using namespace Input;

Player::Player(Mesh* m, Material* mat)
	: Program(m, mat)
{
	ableToFire = true;
	isActive = true;
	numTimesHit = 0;
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

void Player::Draw(ID3D11DeviceContext* context) 
{
	if (isActive) GameObject::Draw(context);
}