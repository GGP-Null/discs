#pragma once
#include "Program.h"
#include "Input.h"

class Disc;

class Player: public Program
{
public:
	Player(Mesh* m, Material* mat);
	~Player();
	virtual void Update(FrameUpdateData);
	void Fire(Disc* d);
	void ReloadDisc();
	void SetNum(int num);
	int GetNum();
private:
	Input::GamePad gamePad;
	Input::Key moveLeft, moveRight, rotLeft, rotRight, fire;
	int playerNum;
	bool ableToFire;
};