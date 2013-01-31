#pragma once
#include "Shader.h"

class ShadowEffect : public Shader
{
	void GetHandles();
	bool ValidateTechniques();

public:
	D3DXHANDLE _depthMapHandle;
	D3DXHANDLE _cubicShadowMappingHandle;
	D3DXHANDLE _ambientHandle;
	D3DXHANDLE _worldMatHandle;
	D3DXHANDLE _worldViewProjMatHandle;
	D3DXHANDLE _cubeShadowMapHandle;
	D3DXHANDLE _eyePositionHandle;
	D3DXHANDLE _lightPositionHandle;

	ShadowEffect(LPDIRECT3DDEVICE9* device, std::string effectFileName) 
		: Shader(device, effectFileName)
	{

	}
	~ShadowEffect()
	{

	}

	bool SetUp();
};
