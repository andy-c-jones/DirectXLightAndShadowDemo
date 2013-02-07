#include "Light.h"

void Light::SetCameraToPositiveX()
{
	SetLook(&_positiveLookX);
	SetUp(&_positiveLookY);
	SetRight(&_negativeLookZ);
	UpdateViewProjectionMatrix();
}

void Light::SetCameraToPositiveY()
{
	SetLook(&_positiveLookY);
	SetUp(&_negativeLookZ);
	SetRight(&_positiveLookX);
	UpdateViewProjectionMatrix();
}

void Light::SetCameraToPositiveZ()
{
	SetLook(&_positiveLookZ);
	SetUp(&_positiveLookY);
	SetRight(&_positiveLookX);
	UpdateViewProjectionMatrix();
}

void Light::SetCameraToNegativeX()
{
	SetLook(&_negativeLookX);
	SetUp(&_positiveLookY);
	SetRight(&_positiveLookZ);
	UpdateViewProjectionMatrix();
}

void Light::SetCameraToNegativeY()
{
	SetLook(&_negativeLookY);
	SetUp(&_negativeLookZ);
	SetRight(&_negativeLookX);
	UpdateViewProjectionMatrix();
}

void Light::SetCameraToNegativeZ()
{
	SetLook(&_negativeLookZ);
	SetUp(&_positiveLookY);
	SetRight(&_negativeLookX);
	UpdateViewProjectionMatrix();
}