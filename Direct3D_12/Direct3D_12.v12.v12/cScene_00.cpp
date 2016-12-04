#include "StdAfx.h"
#include "cScene_00.h"
#include "cTerrain.h"

cScene_00::cScene_00(void)
{
}


cScene_00::~cScene_00(void)
{
}

HRESULT cScene_00::Scene_Init()
{
	this->SetEnvironment("../../Resources/TextureCUBE/CubeMap2.dds");

	//메쉬생성
	D3DXCreateTeapot(Device, &pMesh, NULL);

	return S_OK;
}

void cScene_00::Scene_Release()
{
	SAFE_RELEASE(pMesh);
}

void cScene_00::Scene_Update(float timDelta)
{

}

void cScene_00::Scene_Render1()
{
	meshTrans.SetDeviceWorld(Device);
	this->pMesh->DrawSubset(0);
}