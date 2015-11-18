#include "Input.h"

using namespace Input;

void Input::Update()
{
	UpdateKeyboard();
	UpdateMouse();
	UpdateGamepads();
}