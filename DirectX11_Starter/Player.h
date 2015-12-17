#pragma once
#include "Program.h"
#include "Input.h"

using namespace Input;

class Disc;

class Player: public Program
{
public:
	Player(Mesh* m, Material* mat, Key lKey, Key rKey, Key cwKey, Key ccwKey, Key fireKey);
	~Player();
	virtual void Update(FrameUpdateData);
	void Fire(Disc* d);
	void ReloadDisc();
	void SetNum(int num);
	int GetNum();
	void OnDiscHit();
	virtual void Draw(ID3D11DeviceContext* context) override;
	Input::Key fire;
	int playerNum;

private:
	Input::GamePad gamePad;
	Input::Key moveLeft, moveRight, rotLeft, rotRight;
	bool ableToFire;
	int numTimesHit;
	bool isActive;
};