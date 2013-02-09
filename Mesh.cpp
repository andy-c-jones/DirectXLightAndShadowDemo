#include "Mesh.h"

Mesh::Mesh(IDirect3DDevice9* device, D3DXVECTOR3& position, std::string meshFileName)
{
	_pd3dDevice = device;
	D3DXMatrixIdentity(&_worldMatrix);
	D3DXMatrixTranslation(&_worldMatrix, position.x, position.y, position.z);
	_meshFileName = meshFileName;
	_pMesh = NULL;
	_matBuffer = NULL;
}

Mesh::~Mesh()
{
}

D3DXMATRIXA16* Mesh::GetWorldMat()
{
	return &_worldMatrix;
}

void Mesh::Translate(float inX, float inY, float inZ)
{
	D3DXMatrixIdentity(&_worldMatrix);
	D3DXMatrixTranslation(&_worldMatrix, inX, inY, inZ);
}

bool Mesh::Load()
{
	if( FAILED( D3DXLoadMeshFromX( _meshFileName.c_str(), D3DXMESH_MANAGED,	_pd3dDevice, NULL, &_matBuffer, NULL, &_numMaterials, &_pMesh)))
	{
		if( FAILED( D3DXLoadMeshFromX( _meshFileName.c_str(), D3DXMESH_MANAGED,	_pd3dDevice, NULL, &_matBuffer, NULL, &_numMaterials, &_pMesh)))
		{
			return false;
		}
	}
	return true;
}

void Mesh::RenderAmbient(D3DXMATRIXA16* viewProjectionMatrix, ShadowEffect* shadowMapper)
{
	D3DXMATRIXA16 worldViewProjMat;
	D3DXMatrixMultiply(&worldViewProjMat, &_worldMatrix, viewProjectionMatrix);
	shadowMapper->Effect->SetMatrix(shadowMapper->WorldViewProjMatHandle, &worldViewProjMat);

	shadowMapper->Effect->BeginPass(0);
	_pMesh->DrawSubset(0);
	shadowMapper->Effect->EndPass();
}

void Mesh::RenderMeshWithShadowCube(D3DXMATRIXA16* viewProjectionMatrix, ShadowEffect* shadowMapper)
{
	D3DXMATRIXA16 worldViewProjMat;
	D3DXMatrixMultiply(&worldViewProjMat, &_worldMatrix, viewProjectionMatrix);

	shadowMapper->Effect->SetMatrix(shadowMapper->WorldViewProjMatHandle, &worldViewProjMat);
	shadowMapper->Effect->SetMatrix(shadowMapper->WorldMatrixHandle, &_worldMatrix);

	shadowMapper->Effect->BeginPass(0);
	_pMesh->DrawSubset(0);
	shadowMapper->Effect->EndPass();
}

void Mesh::CleanUp()
{
	if( _pMesh != NULL )
	{
		_pMesh->Release();
		_pMesh = NULL;
	}
}