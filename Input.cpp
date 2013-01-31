#include "Input.h"

bool CInput::Initialise(HWND hwnd, HINSTANCE hInst)
{
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInputObject, NULL)))
	{
		if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInputObject, NULL)))
		{
			return false;
		}
	}

	if(FAILED(_directInputObject->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL)))
	{
		if(FAILED(_directInputObject->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL)))
		{
			return false;
		}
	}

	if(FAILED(_keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		if(FAILED(_keyboard->SetDataFormat(&c_dfDIKeyboard)))
		{
			return false;
		}
	}
	if(FAILED(_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		if(FAILED(_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			return false;
		}
	}

	if(FAILED(_directInputObject->CreateDevice(GUID_SysMouse, &_mouse, NULL)))
	{
		if(FAILED(_directInputObject->CreateDevice(GUID_SysMouse, &_mouse, NULL)))
		{
			return false;
		}
	}

	if(FAILED(_mouse->SetDataFormat(&c_dfDIMouse)))
	{
		if(FAILED(_mouse->SetDataFormat(&c_dfDIMouse)))
		{
			return false;
		}
	}

	if(FAILED(_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
	{
		if(FAILED(_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			return false;
		}
	}

	return true;
}

void CInput::GetInputData()
{
	_keyboard->Acquire();
	_hr = _keyboard->GetDeviceState(sizeof(_keyBuffer), (LPVOID)&_keyBuffer); 

	if(_hr != DI_OK)
	{
		_keyboard->Acquire();
		while(_hr == DIERR_INPUTLOST)
		{
			_hr = _keyboard->Acquire();
		}
	}

	_mouse->Acquire();
	_hr = _mouse->GetDeviceState(sizeof(_mouseState), (LPVOID)&_mouseState); 
	if(_hr == DIERR_INPUTLOST)
	{
		_mouse->Acquire();
	}
}

int CInput::IsWPressed()
{
	return KEYDOWN(_keyBuffer, DIK_W);
}

int CInput::IsSPressed()
{
	return KEYDOWN(_keyBuffer, DIK_S);
}

int CInput::IsAPressed()
{
	return KEYDOWN(_keyBuffer, DIK_A);
}

int CInput::IsDPressed()
{
	return KEYDOWN(_keyBuffer, DIK_D);
}

void CInput::CleanUp()
{
	if(_directInputObject != NULL)
	{
		_directInputObject->Release();
		_directInputObject = NULL;
	}

	if(_keyboard != NULL)
	{
		_keyboard->Release();
		_keyboard = NULL;
	}

	if(_mouse != NULL)
	{
		_mouse->Release();
		_mouse = NULL;
	}
}
