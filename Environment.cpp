#include "Environment.h"

Environment::Environment(Input* input)
{
	_pD3D = NULL;
	_pd3dDevice = NULL;
	_pBackBufferSurface = NULL;

	_pInput = NULL;  
	_pMainCamera = NULL;
	_pLight[0] = NULL;
	_pLight[1] = NULL;
	_pLight[2] = NULL;

	_pShadowEffect = NULL;

	_pTeapot = NULL;
	_pSphere = NULL;
	_pGround = NULL;
	_pCeiling = NULL;

	_lightMoveSpeed = 0.01f;
	_font = NULL;
	_fontDesc = D3DXFONT_DESC();

	_fontPosition.top = 10;
	_fontPosition.left = 10;
	_fontPosition.right = 200;
	_fontPosition.bottom = 200;
	_pInput = input;
}

Environment::~Environment()
{
	CleanUp();
}

bool Environment::InitialiseDirectX( HWND hWnd, UINT screenWidth, UINT screenHeight, BOOL windowed )
{
	UINT AdapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

	if( NULL == ( _pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return false;
	}
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferWidth = screenWidth;
	d3dpp.BackBufferHeight = screenHeight;
	d3dpp.Windowed = windowed;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_FLIP;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	int VP = 0;
	D3DCAPS9 caps;
	_pD3D->GetDeviceCaps(AdapterToUse, DeviceType, &caps);
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		VP = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		VP = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if (FAILED(_pD3D->CreateDevice( AdapterToUse, 
		DeviceType, 
		hWnd, 
		VP, 
		&d3dpp, 
		&_pd3dDevice) ) ) 
	{ 
		MessageBox(hWnd,"Failed To Create Device","BOOM!",MB_OK);
		return false;
	}
	_pd3dDevice->GetRenderTarget(0, &_pBackBufferSurface);

	return true;
}

bool Environment::Initialise( HWND hWnd, HINSTANCE instance, UINT screenWidth, UINT screenHeight, BOOL windowed )
{
	if( !(InitialiseDirectX(hWnd, screenWidth, screenHeight, windowed)) )
	{
		MessageBoxA(NULL, "Direct3d initialisation failed.", NULL, MB_OK);
		return false;
	}

	D3DXCreateFontIndirect(_pd3dDevice,&_fontDesc,&_font);

	D3DXVECTOR3 initialCamPos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);

	for(int i = 0; i < 3; i++)
	{
		_pLight[i] = new Light(_pd3dDevice, &initialCamPos, (D3DX_PI / 2.0f), 1.0f, 1.0f, 500.0f);
	}

	_pShadowEffect = new ShadowEffect(&_pd3dDevice, "shadowEffect.fx");
	if( !(_pShadowEffect->SetUp()) )
	{
		MessageBoxA(NULL, "loading and initialising the shadowing effect failed", "BOOM!", MB_OK);
		return false;
	}

	_pMainCamera = new PlayerCamera(&initialCamPos, D3DX_PI / 2.0f, ((float)screenWidth / (float)screenHeight), 
									1.0f, 500.0f, 20.0f, _pInput);


	D3DXVECTOR3 teapotPos = D3DXVECTOR3(0.0f, 0.0f, 30.0f);
	D3DXVECTOR3 spherePos = D3DXVECTOR3(0.0f, 40.0f, 50.0f);
	D3DXVECTOR3 groundPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ceilingPos = D3DXVECTOR3(0.0f, 70.0f, 0.0f);
	D3DXVECTOR3 wallPos = D3DXVECTOR3(30.0f, 10.0f, 50.0f);

	D3DXVECTOR4 lightPos = D3DXVECTOR4(0.0f, 20.0f, 0.0f, 1.0f);
	D3DXVECTOR4 lightPos2 = D3DXVECTOR4(50.0f, 20.0f, 100.0f, 1.0f);
	D3DXVECTOR4 lightPos3 = D3DXVECTOR4(-100.0f, 20.0f, 0.0f, 1.0f);
	_pTeapot = new Mesh(_pd3dDevice, teapotPos, "teapot.x");
	if( !(_pTeapot->Load()) )
	{
		MessageBoxA(NULL, "loading teapot mesh failed.", "BOOM!", MB_OK);
		return false;
	}
	_pSphere = new Mesh(_pd3dDevice, spherePos, "sphere.x");
	if( !(_pSphere->Load()) )
	{
		MessageBoxA(NULL, "loading sphere mesh failed.", "BOOM!", MB_OK);
		return false;
	}
	_pCeiling = new Mesh(_pd3dDevice, ceilingPos, "roof.x");
	if( !(_pCeiling->Load()) )
	{
		MessageBoxA(NULL, "loading ceiling mesh failed.", "BOOM!", MB_OK);
		return false;
	}

	_pGround = new Mesh(_pd3dDevice, groundPos, "plane.x");
	if( !(_pGround->Load()) )
	{
		MessageBoxA(NULL, "loading ground mesh failed.", "BOOM!", MB_OK);
		return false;
	}

	_pWall = new Mesh(_pd3dDevice, wallPos, "Wall1.x");
	if( !(_pWall->Load()) )
	{
		MessageBoxA(NULL, "loading ground mesh failed.", "BOOM!", MB_OK);
		return false;
	}


	_lightPosition[0] = lightPos;
	_lightPosition[1] = lightPos2;
	_lightPosition[2] = lightPos3;

	return true;
}

void Environment::OnFrameMove(DWORD inTimeDelta)
{
	_pInput->GetInputData();
	_pMainCamera->UpdateCamera((float)(inTimeDelta / 1000.0f));

	if (_lightPosition[0].x > 75 || _lightPosition[0].x < -75)
	{
		_lightMoveSpeed = -_lightMoveSpeed;
	}

	_lightPosition[0].x += _lightMoveSpeed;

	_lightPosition[1].z += _lightMoveSpeed;

	_lightPosition[2].x += _lightMoveSpeed;
	_lightPosition[2].z += _lightMoveSpeed;

	_pShadowEffect->Effect->SetVectorArray(_pShadowEffect->LightPositionHandle, _lightPosition, 3);

	for(int i = 0; i < 3; i++)
	{
		_pLight[i]->SetPosition(&D3DXVECTOR3(_lightPosition[i].x,_lightPosition[i].y,_lightPosition[i].z));
	}

	_pShadowEffect->Effect->SetVector(_pShadowEffect->EyePositionHandle, _pMainCamera->GetPosition4());
}

void Environment::RenderDepthToCubeFace(Light* light, IDirect3DSurface9* cubeFaceSurface)
{
	D3DXMATRIXA16 worldViewProjectionMatrix;
	if(SUCCEEDED(_pd3dDevice->SetRenderTarget( 0, cubeFaceSurface )))
	{
		_pd3dDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, NULL);
	}
	else
	{
		return;
	}

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pCeiling->GetWorldMat(), light->GetViewProjectionMatrix());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pCeiling->GetWorldMat());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->Effect->BeginPass(0);
	_pCeiling->_pMesh->DrawSubset(0);
	_pShadowEffect->Effect->EndPass();

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pWall->GetWorldMat(), light->GetViewProjectionMatrix());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pWall->GetWorldMat());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->Effect->BeginPass(0);
	_pWall->_pMesh->DrawSubset(0);
	_pShadowEffect->Effect->EndPass();

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pGround->GetWorldMat(), light->GetViewProjectionMatrix());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pGround->GetWorldMat());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->Effect->BeginPass(0);
	_pGround->_pMesh->DrawSubset(0);
	_pShadowEffect->Effect->EndPass();

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pTeapot->GetWorldMat(), light->GetViewProjectionMatrix());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pTeapot->GetWorldMat());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->Effect->BeginPass(0);
	_pTeapot->_pMesh->DrawSubset(0);
	_pShadowEffect->Effect->EndPass();

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pSphere->GetWorldMat(), light->GetViewProjectionMatrix());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pSphere->GetWorldMat());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->Effect->BeginPass(0);
	_pSphere->_pMesh->DrawSubset(0);
	_pShadowEffect->Effect->EndPass();
}

void Environment::FillCubicShadowMap(Light* light)
{
	UINT numOfPasses;
	if( FAILED(_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE , D3DCOLORWRITEENABLE_RED )) )
	{
		return;
	}
	_pShadowEffect->Effect->SetVector(_pShadowEffect->ShadowPositionHandle, light->GetPosition4());
	_pShadowEffect->Effect->SetTechnique(_pShadowEffect->DepthMapHandle);
	_pShadowEffect->Effect->Begin(&numOfPasses, NULL);

	light->SetCameraToPositiveX();
	RenderDepthToCubeFace(light, light->_depthCubeFacePX);
	light->SetCameraToPositiveY();
	RenderDepthToCubeFace(light, light->_depthCubeFacePY);
	light->SetCameraToPositiveZ();
	RenderDepthToCubeFace(light, light->_depthCubeFacePZ);
	light->SetCameraToNegativeX();
	RenderDepthToCubeFace(light, light->_depthCubeFaceNX);
	light->SetCameraToNegativeY();
	RenderDepthToCubeFace(light, light->_depthCubeFaceNY);
	light->SetCameraToNegativeZ();
	RenderDepthToCubeFace(light, light->_depthCubeFaceNZ);

	_pShadowEffect->Effect->End();

	if( FAILED(_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_ALPHA | 
		D3DCOLORWRITEENABLE_RED | 
		D3DCOLORWRITEENABLE_GREEN | 
		D3DCOLORWRITEENABLE_BLUE)) )
	{
		return;
	}
}

void Environment::RenderSceneWithShadowMap()
{
	UINT numOfPasses;

	if( SUCCEEDED(_pd3dDevice->SetRenderTarget(0, _pBackBufferSurface)) )
	{
		_pd3dDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, NULL);
	}
	_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);   


	_pShadowEffect->Effect->SetTexture(_pShadowEffect->CubeShadowMapHandle, _pLight[0]->CubicShadowMap);
	_pShadowEffect->Effect->SetTexture(_pShadowEffect->CubeShadowMap2Handle, _pLight[1]->CubicShadowMap);
	_pShadowEffect->Effect->SetTexture(_pShadowEffect->CubeShadowMap3Handle, _pLight[2]->CubicShadowMap);
	_pShadowEffect->Effect->SetTechnique(_pShadowEffect->CubicShadowMappingHandle);

	_pShadowEffect->Effect->Begin(&numOfPasses, NULL);
	_pSphere->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);
	_pTeapot->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);
	_pCeiling->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);
	_pGround->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);
	_pWall->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);
	_pShadowEffect->Effect->End();

	_pShadowEffect->Effect->SetTechnique(_pShadowEffect->AmbientHandle);
}

void Environment::Render(DWORD inTimeDelta, std::string fps)
{
	OnFrameMove(inTimeDelta);

	if( SUCCEEDED(_pd3dDevice->BeginScene()) )
	{
		for (int i = 0; i < 3; i++)
		{
			FillCubicShadowMap(_pLight[i]);
		}
		RenderSceneWithShadowMap();

		_font->DrawText(NULL,
			fps.c_str(),
			-1,
			&_fontPosition,
			DT_LEFT,
			0xffffffff);
	}
	_pd3dDevice->EndScene();

	_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void Environment::CleanUp()
{
	if( _pD3D != NULL )
	{
		_pD3D->Release();
		_pD3D = NULL;
	}
	if( _pd3dDevice != NULL )
	{
		_pd3dDevice->Release();
		_pd3dDevice = NULL;
	}
	if( _pBackBufferSurface != NULL )
	{
		_pBackBufferSurface->Release();
		_pBackBufferSurface = NULL;
	}
	if( _pInput != NULL )
	{
		_pInput->CleanUp();
		delete _pInput;
		_pInput = NULL;
	}
	if( _pMainCamera != NULL )
	{
		delete _pMainCamera;
		_pMainCamera = NULL;
	}

	if( _pLight[0] != NULL )
	{
		delete _pLight[0];
		_pLight[0] = NULL;
	}

	if( _pLight[1] != NULL )
	{
		delete _pLight[1];
		_pLight[1] = NULL;
	}
	if( _pLight[2] != NULL )
	{
		delete _pLight[2];
		_pLight[2] = NULL;
	}

	if( _pShadowEffect != NULL )
	{
		_pShadowEffect->CleanUp();
		delete _pShadowEffect;
		_pShadowEffect = NULL;
	}

	if( _pTeapot != NULL )
	{
		_pTeapot->CleanUp();
		delete _pTeapot;
		_pTeapot = NULL;
	}
	if( _pSphere != NULL )
	{
		_pSphere->CleanUp();
		delete _pSphere;
		_pSphere = NULL;
	}
	if( _pGround != NULL )
	{
		_pGround->CleanUp();
		delete _pGround;
		_pGround = NULL;
	}
	if( _pWall != NULL )
	{
		_pWall->CleanUp();
		delete _pWall;
		_pWall = NULL;
	}
	if( _pCeiling != NULL )
	{
		_pCeiling->CleanUp();
		delete _pCeiling;
		_pCeiling = NULL;
	}
	if(_font != NULL)
	{
		_font->Release();
		_font = NULL;
	}
}