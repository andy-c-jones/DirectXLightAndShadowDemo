#pragma once
#include "Camera.h"

class Light : public Camera
{
public:
	Light(D3DXVECTOR3* position3, float fov, float aspect, float nearZ, float farZ): Camera(position3, fov, aspect, nearZ, farZ){};

	void SetCameraToPositiveX();
	void SetCameraToPositiveY();
	void SetCameraToPositiveZ();
	void SetCameraToNegativeX();
	void SetCameraToNegativeY();
	void SetCameraToNegativeZ();
};