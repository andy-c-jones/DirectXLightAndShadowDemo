#include "Shader.h"

bool Shader::Initialise()
{
	if(FAILED(D3DXCreateEffectFromFile(_pd3dDevice, _effectFileName.c_str(), NULL,  NULL, D3DXSHADER_DEBUG, NULL, &_pEffect, &_pErrorBuffer)))
	{
		if(_pErrorBuffer)
		{
			MessageBoxA(NULL, (LPCSTR)(_pErrorBuffer->GetBufferPointer()), NULL, MB_OK);
		}
		return false;
	}
	return true;
}

void Shader::CleanUp()
{
	if(_pEffect != NULL)
	{
		_pEffect->Release();
		_pEffect = NULL;
	}
	if(_pErrorBuffer != NULL)
	{
		_pErrorBuffer->Release();
		_pErrorBuffer = NULL;
	}
}
