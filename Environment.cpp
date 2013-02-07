#include "Environment.h"

Environment::Environment(Input* input)
{
	_pD3D = NULL;
	_pd3dDevice = NULL;
	_pBackBufferSurface = NULL;

	_pInput = NULL;  
	_pMainCamera = NULL;
	_pLightCamera = NULL;
	_pShadowEffect = NULL;

	_pTeapot = NULL;
	_pSphere = NULL;
	_pGround = NULL;
	_pLight = NULL;

	_cubicShadowMap = NULL;
	_depthCubeFacePX = NULL;
	_depthCubeFacePY = NULL;
	_depthCubeFacePZ = NULL;
	_depthCubeFaceNX = NULL;
	_depthCubeFaceNY = NULL;
	_depthCubeFaceNZ = NULL;
	_lightMoveSpeed = 0.01f;

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

	for (UINT Adapter = 0; Adapter < _pD3D->GetAdapterCount(); Adapter++)
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT Res;
		Res = _pD3D->GetAdapterIdentifier(Adapter, 0, &Identifier);
		if (strstr(Identifier.Description, "PerfHUD") != 0)
		{
			AdapterToUse = Adapter;
			DeviceType = D3DDEVTYPE_REF;
			break;
		}
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferWidth = screenWidth;
	d3dpp.BackBufferHeight = screenHeight;
	d3dpp.Windowed = windowed;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
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
		MessageBoxA(NULL, "Direct3d initialization failed.", NULL, MB_OK);
		return false;
	}

	if( FAILED(D3DXCreateCubeTexture(_pd3dDevice, 512, 1, D3DUSAGE_RENDERTARGET, 
		D3DFMT_R32F, D3DPOOL_DEFAULT, &_cubicShadowMap)) )
	{
		if( FAILED(D3DXCreateCubeTexture(_pd3dDevice, 512, 1, D3DUSAGE_RENDERTARGET, 
			D3DFMT_R16F, D3DPOOL_DEFAULT, &_cubicShadowMap)) )
		{
			MessageBoxA(NULL, "Creating cubic shadow map failed.", "BOOM!", MB_OK);
			return false;
		}

	}

	if( FAILED(_cubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &_depthCubeFacePX)) )
	{
		MessageBoxA(NULL, "Creating cubic shadow map 1 failed.", "BOOM!", MB_OK);
		return false;
	}
	if( FAILED(_cubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &_depthCubeFacePY)) )
	{
		MessageBoxA(NULL, "Creating cubic shadow map 2 failed.", "BOOM!", MB_OK);
		return false;
	}
	if( FAILED(_cubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &_depthCubeFacePZ)) )
	{
		MessageBoxA(NULL, "Creating cubic shadow map 3 failed.", "BOOM!", MB_OK);
		return false;
	}
	if( FAILED(_cubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &_depthCubeFaceNX)) )
	{
		MessageBoxA(NULL, "Creating cubic shadow map 4 failed.", "BOOM!", MB_OK);
		return false;
	}
	if( FAILED(_cubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &_depthCubeFaceNY)) )
	{
		MessageBoxA(NULL, "Creating cubic shadow map 5 failed.", "BOOM!", MB_OK);
		return false;
	}
	if( FAILED(_cubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &_depthCubeFaceNZ)) )
	{
		MessageBoxA(NULL, "Creating cubic shadow map 6 failed.", "BOOM!", MB_OK);
		return false;
	}

	_pShadowEffect = new ShadowEffect(&_pd3dDevice, "shadowEffect.fx");
	if( !(_pShadowEffect->SetUp()) )
	{
		MessageBoxA(NULL, "loading and initializing the shadowing effect failed", "BOOM!", MB_OK);
		return false;
	}

	//_pInput = new Input();
	//if( !(_pInput->Initialise(hWnd, instance)) )
	//{
	//	MessageBoxA(NULL, "Direct Input initialization failed.", "BOOM!", MB_OK);
	//	return false;
	//}

	D3DXVECTOR3 initialCamPos = D3DXVECTOR3(0.0f, 30.0f, 0.0f);
	_pMainCamera = new PlayerCamera(&initialCamPos, D3DX_PI / 2.0f, ((float)screenWidth / (float)screenHeight), 
									1.0f, 500.0f, 20.0f, _pInput);

	_pLightCamera = new Camera(&initialCamPos, (D3DX_PI / 2.0f), 1.0f, 1.0f, 500.0f);

	D3DXVECTOR3 teapotPos = D3DXVECTOR3(0.0f, 0.0f, 30.0f);
	D3DXVECTOR3 spherePos = D3DXVECTOR3(0.0f, 10.0f, 50.0f);
	D3DXVECTOR3 groundPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lightPos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);

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
	_pGround = new Mesh(_pd3dDevice, groundPos, "ground.x");
	if( !(_pGround->Load()) )
	{
		MessageBoxA(NULL, "loading ground mesh failed.", "BOOM!", MB_OK);
		return false;
	}

	_pLight = new Mesh(_pd3dDevice, lightPos, "light.x");
	if( !(_pLight->Load()) )
	{
		MessageBoxA(NULL, "loading light mesh failed.", "BOOM!", MB_OK);
		return false;
	}

	_positiveLookX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_positiveLookY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_positiveLookZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	_negativeLookX = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	_negativeLookY = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	_negativeLookZ = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	_lightPosition = lightPos;

	return true;
}

void Environment::CreateCamForPositiveX(Camera* lightCamera)
{
	lightCamera->SetLook(&_positiveLookX);
	lightCamera->SetUp(&_positiveLookY);
	lightCamera->SetRight(&_negativeLookZ);

	lightCamera->UpdateViewProjectionMatrix();
}

void Environment::CreateCamForNegativeX(Camera* lightCamera)
{
	lightCamera->SetLook(&_negativeLookX);
	lightCamera->SetUp(&_positiveLookY);
	lightCamera->SetRight(&_positiveLookZ);

	lightCamera->UpdateViewProjectionMatrix();
}

void Environment::CreateCamForPositiveY(Camera* lightCamera)
{
	lightCamera->SetLook(&_positiveLookY);
	lightCamera->SetUp(&_negativeLookZ);
	lightCamera->SetRight(&_positiveLookX);

	lightCamera->UpdateViewProjectionMatrix();
}

void Environment::CreateCamForNegativeY(Camera* lightCamera)
{
	lightCamera->SetLook(&_negativeLookY);
	lightCamera->SetUp(&_negativeLookZ);
	lightCamera->SetRight(&_negativeLookX);

	lightCamera->UpdateViewProjectionMatrix();
}

void Environment::CreateCamForPositiveZ(Camera* lightCamera)
{
	lightCamera->SetLook(&_positiveLookZ);
	lightCamera->SetUp(&_positiveLookY);
	lightCamera->SetRight(&_positiveLookX);

	lightCamera->UpdateViewProjectionMatrix();
}

void Environment::CreateCamForNegativeZ(Camera* lightCamera)
{
	lightCamera->SetLook(&_negativeLookZ);
	lightCamera->SetUp(&_positiveLookY);
	lightCamera->SetRight(&_negativeLookX);

	lightCamera->UpdateViewProjectionMatrix();
}

void Environment::OnFrameMove(DWORD inTimeDelta)
{
	_pInput->GetInputData();
	_pMainCamera->UpdateCamera((float)(inTimeDelta / 1000.0f));

	if (_lightPosition.x > 75 || _lightPosition.x < -75)
	{
		_lightMoveSpeed = -_lightMoveSpeed;
	}

	_lightPosition.x += _lightMoveSpeed;

	_pShadowEffect->_pEffect->SetVector(_pShadowEffect->_lightPositionHandle, &D3DXVECTOR4(_lightPosition, 1.0f));
	_pLightCamera->SetPosition(&_lightPosition);
	_pLight->Translate(_lightPosition.x, _lightPosition.y, _lightPosition.z);

	_pShadowEffect->_pEffect->SetVector(_pShadowEffect->_eyePositionHandle, _pMainCamera->GetPosition4());
}

void Environment::RenderDepthToCubeFace(IDirect3DSurface9* cubeFaceSurface)
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

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pGround->GetWorldMat(), _pLightCamera->GetViewProjectionMatrix());
	_pShadowEffect->_pEffect->SetMatrix(_pShadowEffect->_worldMatHandle, _pGround->GetWorldMat());
	_pShadowEffect->_pEffect->SetMatrix(_pShadowEffect->_worldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->_pEffect->BeginPass(0);
	_pGround->_pMesh->DrawSubset(0);
	_pShadowEffect->_pEffect->EndPass();

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pTeapot->GetWorldMat(), _pLightCamera->GetViewProjectionMatrix());
	_pShadowEffect->_pEffect->SetMatrix(_pShadowEffect->_worldMatHandle, _pTeapot->GetWorldMat());
	_pShadowEffect->_pEffect->SetMatrix(_pShadowEffect->_worldViewProjMatHandle, &worldViewProjectionMatrix);

	//_pShadowEffect->_pEffect->BeginPass(0);
	//_pTeapot->_pMesh->DrawSubset(0);
	//_pShadowEffect->_pEffect->EndPass();

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pSphere->GetWorldMat(), _pLightCamera->GetViewProjectionMatrix());
	_pShadowEffect->_pEffect->SetMatrix(_pShadowEffect->_worldMatHandle, _pSphere->GetWorldMat());
	_pShadowEffect->_pEffect->SetMatrix(_pShadowEffect->_worldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->_pEffect->BeginPass(0);
	_pSphere->_pMesh->DrawSubset(0);
	_pShadowEffect->_pEffect->EndPass();
}

void Environment::FillCubicShadowMap()
{
	UINT numOfPasses;
	if( FAILED(_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE , D3DCOLORWRITEENABLE_RED )) )
	{
		return;
	}

	_pShadowEffect->_pEffect->SetTechnique(_pShadowEffect->_depthMapHandle);
	_pShadowEffect->_pEffect->Begin(&numOfPasses, NULL);

	CreateCamForPositiveX(_pLightCamera);
	RenderDepthToCubeFace(_depthCubeFacePX);
	CreateCamForPositiveY(_pLightCamera);
	RenderDepthToCubeFace(_depthCubeFacePY);
	CreateCamForPositiveZ(_pLightCamera);
	RenderDepthToCubeFace(_depthCubeFacePZ);
	CreateCamForNegativeX(_pLightCamera);
	RenderDepthToCubeFace(_depthCubeFaceNX);
	CreateCamForNegativeY(_pLightCamera);
	RenderDepthToCubeFace(_depthCubeFaceNY);
	CreateCamForNegativeZ(_pLightCamera);
	RenderDepthToCubeFace(_depthCubeFaceNZ);

	_pShadowEffect->_pEffect->End();

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

	_pShadowEffect->_pEffect->SetTexture(_pShadowEffect->_cubeShadowMapHandle, _cubicShadowMap);
	_pShadowEffect->_pEffect->SetTechnique(_pShadowEffect->_cubicShadowMappingHandle);

	_pShadowEffect->_pEffect->Begin(&numOfPasses, NULL);
	_pSphere->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);
	//_pTeapot->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);
	_pGround->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);
	_pShadowEffect->_pEffect->End();

	_pShadowEffect->_pEffect->SetTechnique(_pShadowEffect->_ambientHandle);


	//_pShadowEffect->_pEffect->Begin(&numOfPasses, NULL);
	//_pLight->RenderAmbient(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);
	//_pShadowEffect->_pEffect->End();
}

void Environment::Render(DWORD inTimeDelta)
{
	OnFrameMove(inTimeDelta);

	if( SUCCEEDED(_pd3dDevice->BeginScene()) )
	{
		FillCubicShadowMap();
		RenderSceneWithShadowMap();
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
	if( _pLightCamera != NULL )
	{
		delete _pLightCamera;
		_pLightCamera = NULL;
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
	if( _pLight != NULL )
	{
		_pLight->CleanUp();
		delete _pLight;
		_pLight = NULL;
	}

	if( _cubicShadowMap != NULL )
	{
		_cubicShadowMap->Release();
		_cubicShadowMap = NULL;
	}
	if( _depthCubeFacePX != NULL )
	{
		_depthCubeFacePX->Release();
		_depthCubeFacePX = NULL;
	}
	if( _depthCubeFacePY != NULL )
	{
		_depthCubeFacePY->Release();
		_depthCubeFacePY = NULL;
	}
	if( _depthCubeFacePZ != NULL )
	{
		_depthCubeFacePZ->Release();
		_depthCubeFacePZ = NULL;
	}
	if( _depthCubeFaceNX != NULL )
	{
		_depthCubeFaceNX->Release();
		_depthCubeFaceNX = NULL;
	}
	if( _depthCubeFaceNY != NULL )
	{
		_depthCubeFaceNY->Release();
		_depthCubeFaceNY = NULL;
	}
	if( _depthCubeFaceNZ != NULL )
	{
		_depthCubeFaceNZ->Release();
		_depthCubeFaceNZ = NULL;
	}
}