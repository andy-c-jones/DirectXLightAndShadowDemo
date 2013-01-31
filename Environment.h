#pragma once
#include "Input.h"
#include "MainCamera.h"
#include "Mesh.h"

class Environment
{
	LPDIRECT3D9 _pD3D;
	LPDIRECT3DDEVICE9 _pd3dDevice;
	LPDIRECT3DSURFACE9 _pBackBufferSurface;

	CInput* _pInput;
	MainCamera* _pMainCamera;
	Camera* _pLightCamera;
	ShadowEffect* _pShadowEffect;
	D3DXVECTOR3 _lightPosition;

	Mesh* _pTeapot;
	Mesh* _pSphere;
	Mesh* _pGround;
	Mesh* _pLight;

	LPDIRECT3DCUBETEXTURE9 _cubicShadowMap;
	LPDIRECT3DSURFACE9 _depthCubeFacePX;
	LPDIRECT3DSURFACE9 _depthCubeFacePY;
	LPDIRECT3DSURFACE9 _depthCubeFacePZ;
	LPDIRECT3DSURFACE9 _depthCubeFaceNX;
	LPDIRECT3DSURFACE9 _depthCubeFaceNY;
	LPDIRECT3DSURFACE9 _depthCubeFaceNZ;

	D3DXVECTOR3 _positiveLookX;
	D3DXVECTOR3 _positiveLookY;
	D3DXVECTOR3 _positiveLookZ;
	D3DXVECTOR3 _negativeLookX;
	D3DXVECTOR3 _negativeLookY;
	D3DXVECTOR3 _negativeLookZ;

	float _lightMoveSpeed;

	void CreateCamForPositiveX();
	void CreateCamForNegativeX();
	void CreateCamForPositiveY();
	void CreateCamForNegativeY();
	void CreateCamForPositiveZ();
	void CreateCamForNegativeZ();

	bool InitialiseDirectX(HWND hWnd, UINT inScreenWidth, UINT inScreenHeight, BOOL inWindowed);
	
	void OnFrameMove(DWORD inTimeDelta);

	void RenderDepthToCubeFace(LPDIRECT3DSURFACE9 inCubeFaceSurface);
	void FillCubicShadowMap();
	void RenderSceneWithShadowMap();
public:
	Environment();
	~Environment();

	bool Initialise(HWND hWnd, HINSTANCE inInstance, UINT inScreenWidth, UINT inScreenHeight, BOOL inWindowed);
	void Render(DWORD inTimeDelta);
	void CleanUp();
};
