#include "Mesh.h"

Mesh::Mesh(LPDIRECT3DDEVICE9 inDevice, D3DXVECTOR3& inPosition, std::string inMeshFileName)
{
	_pd3dDevice = inDevice;
	D3DXMatrixIdentity(&_worldMatrix);
	D3DXMatrixTranslation(&_worldMatrix, inPosition.x, inPosition.y, inPosition.z);
	_meshFileName = inMeshFileName;
	_pMesh = NULL;
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
	if( FAILED( D3DXLoadMeshFromX( _meshFileName.c_str(), 
		D3DXMESH_MANAGED, 
		_pd3dDevice, 
		NULL, 
		NULL, 
		NULL, 
		&_numMaterials, 
		&_pMesh ) ) )
	{

		if( FAILED( D3DXLoadMeshFromX( _meshFileName.c_str(), 
			D3DXMESH_MANAGED,
			_pd3dDevice, 
			NULL, 
			NULL,
			NULL, 
			&_numMaterials, 
			&_pMesh ) ) )
		{
			return false;
		}
	}

	return true;
}

void Mesh::RenderAmbient(D3DXMATRIXA16* inViewProjMat, ShadowEffect* inShadowMapper)
{
	D3DXMATRIXA16 worldViewProjMat;
	D3DXMatrixMultiply(&worldViewProjMat, &_worldMatrix, inViewProjMat);
	inShadowMapper->_pEffect->SetMatrix(inShadowMapper->_worldViewProjMatHandle, &worldViewProjMat);

	inShadowMapper->_pEffect->BeginPass(0);
	_pMesh->DrawSubset(0);
	inShadowMapper->_pEffect->EndPass();
}

void Mesh::RenderMeshWithShadowCube(D3DXMATRIXA16* inViewProjMat, ShadowEffect* inShadowMapper)
{
	D3DXMATRIXA16 worldViewProjMat;
	D3DXMatrixMultiply(&worldViewProjMat, &_worldMatrix, inViewProjMat);

	inShadowMapper->_pEffect->SetMatrix(inShadowMapper->_worldViewProjMatHandle, &worldViewProjMat);
	inShadowMapper->_pEffect->SetMatrix(inShadowMapper->_worldMatHandle, &_worldMatrix);

	inShadowMapper->_pEffect->BeginPass(0);
	_pMesh->DrawSubset(0);
	inShadowMapper->_pEffect->EndPass();
}

void Mesh::CleanUp()
{
	if( _pMesh != NULL )
	{
		_pMesh->Release();
		_pMesh = NULL;
	}
}