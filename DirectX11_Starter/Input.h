#pragma once

#include <windows.h>
#include "Keyboard.h"
#include "Mouse.h"
#include "GamePad.h"

/// <summary>
/// The Input namespace contains helper functions for input.
/// </summary>
namespace Input {

#pragma region typedefs
	typedef DirectX::Keyboard Keyboard;
	typedef Keyboard::State KeyboardState;
	typedef Keyboard::Keys Keys;
	typedef Keyboard::Keys Key;
	typedef Keyboard::KeyboardStateTracker KeyboardStateTracker;

	typedef DirectX::Mouse Mouse;
	typedef Mouse::Mode MouseMode;
	typedef Mouse::State MouseState;
	typedef Mouse::ButtonStateTracker MouseButtonStateTracker;
	typedef MouseButtonStateTracker::ButtonState MouseButtonState;

	// TODO: Gamepad Stuff
	typedef DirectX::GamePad GamePadManager;
#pragma endregion

	class GamePad
	{
	public:
		typedef GamePadManager::State State;
		typedef GamePadManager::ButtonStateTracker ButtonStateTracker;
		typedef ButtonStateTracker::ButtonState ButtonState;
		static const int MAX_PLAYERS = DirectX::GamePad::MAX_PLAYER_COUNT;

		GamePad();
		static GamePad FromId(int id);
		static GamePad Next();

		bool IsConnected();

		State const& GetState();
		ButtonStateTracker const& GetTrackedState();

		static bool ButtonPressedThisFrame(ButtonState);
		static bool ButtonIsDown(ButtonState);
		static bool ButtonIsHeld(ButtonState);

		static bool ButtonReleasedThisFrame(ButtonState);
		static bool ButtonIsUp(ButtonState);
		static bool ButtonHasBeenUp(ButtonState);

		bool SetVibes(float left, float right, float leftTrig = 0.f, float rightTrig = 0.f);

		int GetId();

	private:
		GamePad(int id);
		int id;
	};

#pragma region Keyboard Functions
	/// <summary>
	/// Determine if the given key was pressed this frame.
	/// </summary>
	/// <param name="key">The key to test against.</param>
	/// <returns>true if down this frame, false if not down or was also down last frame.</return>
	bool KeyPressedThisFrame(Key key);

	/// <summary>
	/// Determine if the given key is down at all.
	/// </summary>
	/// <param name="key">The key to test against</param>
	/// <returns>true if the key is down, false if it is not.</returns>
	bool KeyIsDown(Key key);

	/// <summary>
	/// Determine if the given key was released this frame.
	/// </summary>
	/// <param name="key">The key to test against</param>
	/// <returns>true if the key was released this frame, false if it is down or was released last frame.</returns>
	bool KeyReleasedThisFrame(Key key);

	/// <summary>
	/// Determine if the given key is up.
	/// </summary>
	/// <param name="key">The key to test against</param>
	/// <returns>true if the key is up, false if it is not.</returns>
	bool KeyIsUp(Key key);
#pragma endregion

#pragma region Mouse Movement Functions
	void SetMouseWindow(HWND hWnd);

	void SetMouseMode(MouseMode);
	MouseMode GetMouseMode();

	int GetMouseX();
	int GetMouseY();

	void ResetScrollWheel();
	int GetScrollWheel();

#pragma endregion

#pragma region Mouse Button Functions

	/// <summary>
	/// Determine if the left mouse button was pressed this frame.
	/// </summary>
	/// <returns>true if pressed this frame, false if it is up or was also down last frame.</returns>
	bool LeftMouseDownThisFrame();

	/// <summary>
	/// Determine if the left mouse button is down at all.
	/// </summary>
	/// <returns>true if down at all, false if up at all.</returns>
	bool LeftMouseIsDown();

	/// <summary>
	/// Determine if the left mouse button is held down (down this frame and the last one)
	/// </summary>
	/// <returns>true if down this frame as well as the last one, false if it is up or was only just pressed this frame.</returns>
	bool LeftMouseIsHeld();

	/// <summary>
	/// Determine if the left mouse button was released this frame
	/// </summary>
	/// <returns>true if just released this frame, false if down or released on a prior frame</returns>
	bool LeftMouseReleasedThisFrame();

	/// <summary>
	/// Determine if the left mouse button is up at all
	/// </summary>
	/// <returns>true if up, false if down</returns>
	bool LeftMouseIsUp();


	/// <summary>
	/// Determine if the middle mouse button was pressed this frame.
	/// </summary>
	/// <returns>true if pressed this frame, false if it is up or was also down last frame.</returns>
	bool MiddleMouseDownThisFrame();

	/// <summary>
	/// Determine if the middle mouse button is down at all.
	/// </summary>
	/// <returns>true if down at all, false if up at all.</returns>
	bool MiddleMouseIsDown();

	/// <summary>
	/// Determine if the middle mouse button is held down (down this frame and the last one)
	/// </summary>
	/// <returns>true if down this frame as well as the last one, false if it is up or was only just pressed this frame.</returns>
	bool MiddleMouseIsHeld();

	/// <summary>
	/// Determine if the middle mouse button was released this frame
	/// </summary>
	/// <returns>true if just released this frame, false if down or released on a prior frame</returns>
	bool MiddleMouseReleasedThisFrame();

	/// <summary>
	/// Determine if the middle mouse button is up at all
	/// </summary>
	/// <returns>true if up, false if down</returns>
	bool MiddleMouseIsUp();



	/// <summary>
	/// Determine if the right mouse button was pressed this frame.
	/// </summary>
	/// <returns>true if pressed this frame, false if it is up or was also down last frame.</returns>
	bool RightMouseDownThisFrame();

	/// <summary>
	/// Determine if the right mouse button is down at all.
	/// </summary>
	/// <returns>true if down at all, false if up at all.</returns>
	bool RightMouseIsDown();

	/// <summary>
	/// Determine if the right mouse button is held down (down this frame and the last one)
	/// </summary>
	/// <returns>true if down this frame as well as the last one, false if it is up or was only just pressed this frame.</returns>
	bool RightMouseIsHeld();

	/// <summary>
	/// Determine if the right mouse button was released this frame
	/// </summary>
	/// <returns>true if just released this frame, false if down or released on a prior frame</returns>
	bool RightMouseReleasedThisFrame();

	/// <summary>
	/// Determine if the right mouse button is up at all
	/// </summary>
	/// <returns>true if up, false if down</returns>
	bool RightMouseIsUp();



	/// <summary>
	/// Determine if the X1 mouse button was pressed this frame.
	/// </summary>
	/// <returns>true if pressed this frame, false if it is up or was also down last frame.</returns>
	bool X1MouseDownThisFrame();

	/// <summary>
	/// Determine if the X1 mouse button is down at all.
	/// </summary>
	/// <returns>true if down at all, false if up at all.</returns>
	bool X1MouseIsDown();

	/// <summary>
	/// Determine if the X1 mouse button is held down (down this frame and the last one)
	/// </summary>
	/// <returns>true if down this frame as well as the last one, false if it is up or was only just pressed this frame.</returns>
	bool X1MouseIsHeld();

	/// <summary>
	/// Determine if the X1 mouse button was released this frame
	/// </summary>
	/// <returns>true if just released this frame, false if down or released on a prior frame</returns>
	bool X1MouseReleasedThisFrame();

	/// <summary>
	/// Determine if the X1 mouse button is up at all
	/// </summary>
	/// <returns>true if up, false if down</returns>
	bool X1MouseIsUp();



	/// <summary>
	/// Determine if the X2 mouse button was pressed this frame.
	/// </summary>
	/// <returns>true if pressed this frame, false if it is up or was also down last frame.</returns>
	bool X2MouseDownThisFrame();

	/// <summary>
	/// Determine if the X2 mouse button is down at all.
	/// </summary>
	/// <returns>true if down at all, false if up at all.</returns>
	bool X2MouseIsDown();

	/// <summary>
	/// Determine if the X2 mouse button is held down (down this frame and the last one)
	/// </summary>
	/// <returns>true if down this frame as well as the last one, false if it is up or was only just pressed this frame.</returns>
	bool X2MouseIsHeld();

	/// <summary>
	/// Determine if the X2 mouse button was released this frame
	/// </summary>
	/// <returns>true if just released this frame, false if down or released on a prior frame</returns>
	bool X2MouseReleasedThisFrame();

	/// <summary>
	/// Determine if the X2 mouse button is up at all
	/// </summary>
	/// <returns>true if up, false if down</returns>
	bool X2MouseIsUp();


#pragma endregion

#pragma region Updates
	/// <summary>
	/// Update keyboard state tracking.
	/// </summary>
	void UpdateKeyboard();

	void UpdateMouse();

	void UpdateGamepads();

	void Update();
#pragma endregion
};