#pragma once
#include "Shader.h"

class ShadowEffect : public Shader
{
	void GetHandles();
	bool ValidateTechniques();

public:
	D3DXHANDLE DepthMapHandle;
	D3DXHANDLE CubicShadowMappingHandle;
	D3DXHANDLE AmbientHandle;
	D3DXHANDLE WorldMatrixHandle;
	D3DXHANDLE WorldViewProjMatHandle;
	D3DXHANDLE CubeShadowMapHandle;
	D3DXHANDLE EyePositionHandle;
	D3DXHANDLE LightPositionHandle;
	D3DXHANDLE LightNumberHandle;

	ShadowEffect(LPDIRECT3DDEVICE9* device, std::string effectFileName) 
		: Shader(device, effectFileName)
	{

	}
	~ShadowEffect()
	{

	}

	bool SetUp();
};
