#pragma once
#include "Input.h"
#include "PlayerCamera.h"
#include "Mesh.h"
#include "Light.h"

class Environment
{
	IDirect3D9* _pD3D;
	IDirect3DDevice9* _pd3dDevice;
	IDirect3DSurface9* _pBackBufferSurface;

	Input* _pInput;
	PlayerCamera* _pMainCamera;
	Light* _pLight;
	ShadowEffect* _pShadowEffect;
	D3DXVECTOR3 _lightPosition;

	Mesh* _pTeapot;
	Mesh* _pSphere;
	Mesh* _pGround;
	Mesh* _pLightMesh;



	LPD3DXFONT _font; 
	D3DXFONT_DESC _fontDesc;
	RECT _fontPosition;

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
	void Render(DWORD inTimeDelta, std::string fps);
	void CleanUp();
};
