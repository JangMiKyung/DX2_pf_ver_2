#include "StdAfx.h"
#include "cScene_00.h"
#include "cTerrain.h"

cScene_00::cScene_00(void)
{
}


cScene_00::~cScene_00(void)
{
}

HRESULT cScene_00::Init()
{
	m_pTerrain = new cTerrain();
	m_pTerrain->Init(1024, 1.0f);



	return S_OK;

}

void cScene_00::Release()
{
	m_pTerrain->Release();
	SAFE_DELETE(m_pTerrain);
}

void cScene_00::Update(float timeDelta)
{
	m_mainCamera.DefaultControl(timeDelta);
	m_mainCamera.UpdateCamToDevice(Device);

}

void cScene_00::Render()
{
	m_pTerrain->Render();
}

