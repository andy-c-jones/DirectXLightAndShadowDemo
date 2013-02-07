#pragma once
#include "Input.h"
#include "PlayerCamera.h"
#include "Mesh.h"

class Environment
{
	IDirect3D9* _pD3D;
	IDirect3DDevice9* _pd3dDevice;
	IDirect3DSurface9* _pBackBufferSurface;

	Input* _pInput;
	PlayerCamera* _pMainCamera;
	Camera* _pLightCamera;
	ShadowEffect* _pShadowEffect;
	D3DXVECTOR3 _lightPosition;

	Mesh* _pTeapot;
	Mesh* _pSphere;
	Mesh* _pGround;
	Mesh* _pLight;

	IDirect3DCubeTexture9* _cubicShadowMap;
	IDirect3DSurface9* _depthCubeFacePX;
	IDirect3DSurface9* _depthCubeFacePY;
	IDirect3DSurface9* _depthCubeFacePZ;
	IDirect3DSurface9* _depthCubeFaceNX;
	IDirect3DSurface9* _depthCubeFaceNY;
	IDirect3DSurface9* _depthCubeFaceNZ;



	float _lightMoveSpeed;

	bool InitialiseDirectX(HWND hWnd, UINT inScreenWidth, UINT inScreenHeight, BOOL inWindowed);
	
	void OnFrameMove(DWORD inTimeDelta);

	void RenderDepthToCubeFace(IDirect3DSurface9* cubeFaceSurface);
	void FillCubicShadowMap();
	void RenderSceneWithShadowMap();
public:
	Environment(Input* input);
	~Environment();

	bool Initialise(HWND hWnd, HINSTANCE inInstance, UINT inScreenWidth, UINT inScreenHeight, BOOL inWindowed);
	void Render(DWORD inTimeDelta);
	void CleanUp();
};
