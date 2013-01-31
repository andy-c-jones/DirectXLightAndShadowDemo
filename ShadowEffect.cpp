#include "ShadowEffect.h"

void ShadowEffect::GetHandles()
{
	_depthMapHandle = _pEffect->GetTechniqueByName("depthMap");
	_cubicShadowMappingHandle = _pEffect->GetTechniqueByName("cubicShadowMapping");
	_ambientHandle = _pEffect->GetTechniqueByName("ambient");
	_worldMatHandle = _pEffect->GetParameterByName(NULL, "worldMat");
	_worldViewProjMatHandle = _pEffect->GetParameterByName(NULL, "worldViewProjMat");
	_cubeShadowMapHandle = _pEffect->GetParameterByName(NULL, "cubeShadowMap");
	_eyePositionHandle = _pEffect->GetParameterByName(NULL, "eyePosition");
	_lightPositionHandle = _pEffect->GetParameterByName(NULL, "lightPosition");
}

bool ShadowEffect::ValidateTechniques()
{
	if(FAILED(_pEffect->ValidateTechnique(_depthMapHandle)))
	{
		MessageBoxA(NULL, "depth map technique validation failed.", NULL, MB_OK);
		return false;
	}
	if(FAILED(_pEffect->ValidateTechnique(_cubicShadowMappingHandle)))
	{
		MessageBoxA(NULL, "cubic shadow mapping technique validation failed.", NULL, MB_OK);
		return false;
	}
	if(FAILED(_pEffect->ValidateTechnique(_ambientHandle)))
	{
		MessageBoxA(NULL, "ambient technique validation failed.", NULL, MB_OK);
		return false;
	}

	return true;
}

bool ShadowEffect::SetUp()
{
	if(!(Initialise()) )
	{
		return false;
	}

	GetHandles();

	if( !(ValidateTechniques()) )
	{
		return false;
	}

	return true;
}