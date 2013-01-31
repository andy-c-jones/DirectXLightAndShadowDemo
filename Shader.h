#pragma once
#include <string>
#include "d3dx9.h"

class Shader
{
protected:
	LPD3DXBUFFER _pErrorBuffer;
	LPDIRECT3DDEVICE9 _pd3dDevice;
	std::string _effectFileName;

public:
	LPD3DXEFFECT _pEffect;

	Shader(LPDIRECT3DDEVICE9* InDevice, std::string inEffectFileName) 
	{
		_pd3dDevice = *InDevice;
		_effectFileName = inEffectFileName;
		_pEffect = NULL;
		_pErrorBuffer = NULL;
	}

	~Shader( )
	{
		CleanUp();
	}

	bool Initialise();  
	void CleanUp();
};
