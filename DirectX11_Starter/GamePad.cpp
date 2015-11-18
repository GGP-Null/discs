#include "Input.h"
#include <vector>

using namespace Input;
using std::vector;
#define MAX_PLAYERS GamePad::MAX_PLAYERS

GamePadManager theGamePadMgr;

vector<bool> activeGamePads(MAX_PLAYERS);
vector<GamePad::State> padStates(MAX_PLAYERS);
vector<GamePad::ButtonStateTracker> trackedPadStates(MAX_PLAYERS);

void updateGamePad(int i)
{
	auto newState = theGamePadMgr.GetState(i);
	padStates[i] = newState;
	trackedPadStates[i].Update(newState);
}

void Input::UpdateGamepads()
{
	for (auto i = 0; i < MAX_PLAYERS; ++i)
		if (activeGamePads[i]) {
			updateGamePad(i);
			// for debug breaking
		}
}

GamePad::GamePad() :
	id()
{
}

GamePad::GamePad(int id) :
	id(id)
{
}

GamePad GamePad::FromId(int id)
{
	updateGamePad(id);
	activeGamePads[id] = true;
	return GamePad(id);
}

GamePad GamePad::Next()
{
	for (auto i = 0; i < MAX_PLAYERS; ++i)
		if (!activeGamePads[i]) return FromId(i);
	
	return FromId(0);
}

int GamePad::GetId()
{
	return id;
}

GamePad::State const& GamePad::GetState()
{
	return padStates[id];
}

bool GamePad::IsConnected()
{
	return padStates[id].IsConnected();
}

GamePad::ButtonStateTracker const& GamePad::GetTrackedState()
{
	return trackedPadStates[id];
}

bool GamePad::ButtonPressedThisFrame(ButtonState btn)
{
	return btn == ButtonState::PRESSED;
}

bool GamePad::ButtonIsDown(ButtonState btn)
{
	return btn == ButtonState::HELD || btn == ButtonState::PRESSED;
}

bool GamePad::ButtonIsHeld(ButtonState btn)
{
	return btn == ButtonState::HELD;
}

bool GamePad::ButtonReleasedThisFrame(ButtonState btn)
{
	return btn == ButtonState::RELEASED;
}

bool GamePad::ButtonIsUp(ButtonState btn)
{
	return btn == ButtonState::RELEASED || btn == ButtonState::UP;
}

bool GamePad::ButtonHasBeenUp(ButtonState btn)
{
	return btn == ButtonState::UP;
}

bool GamePad::SetVibes(float left, float right, float leftTrig, float rightTrig)
{
	return theGamePadMgr.SetVibration(id, left, right, leftTrig, rightTrig);
}