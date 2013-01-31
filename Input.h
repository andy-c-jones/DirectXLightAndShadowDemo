#pragma once
#include <Windows.h>
#include <dinput.h>
#include "d3dx9.h"

#define KEYDOWN(name, key) (name[key] & 0x80)

class CInput
{
	LPDIRECTINPUT8 _directInputObject;
	LPDIRECTINPUTDEVICE8 _keyboard;
	LPDIRECTINPUTDEVICE8 _mouse;

	HRESULT _hr;

public:

	char _keyBuffer[256];
	DIMOUSESTATE _mouseState;

	CInput()
	{
		_directInputObject = NULL;
		_keyboard = NULL;
		_mouse = NULL;
	}

	~CInput()
	{
	}

	bool Initialise(HWND hwnd, HINSTANCE hInst);
	void GetInputData();

	int IsWPressed();
	int IsSPressed();
	int IsAPressed();
	int IsDPressed();

	void CleanUp();
};
