#pragma once
#include "Camera.h"
#include "Input.h"

class MainCamera : public Camera
{
	CInput* _pInput;
	float _moveSpeed;
	D3DXMATRIXA16 tmpMat;

public:

	MainCamera(D3DXVECTOR3* inPosition3, float inFov, float inAspect, float inNearZ, float inFarZ, float inMoveSpeed, CInput* inInput) 
		: Camera(inPosition3, inFov, inAspect, inNearZ, inFarZ)
	{
		_moveSpeed = inMoveSpeed;
		_pInput = inInput;
	}

	~MainCamera()
	{
	}

	float GetMoveSpeed();
	void SetMoveSpeed(float inMoveSpeed);
	void MoveForwards(float inTimeDelta);
	void MoveBackwards(float inTimeDelta);
	void StrafeRight(float inTimeDelta);
	void StrafeLeft(float inTimeDelta);
	void UpdateCamera(float inTimeDelta);
	void UpdateViewProjLastMat();
};
