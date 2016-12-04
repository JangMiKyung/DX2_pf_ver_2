#include "stdafx.h"
#include "cScene_00.h"


cScene_00::cScene_00()
{
}


cScene_00::~cScene_00()
{
}

void cScene_00::Init()
{
	//뷰행렬 준비
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 eyePos(5.f, 10.f, -20.f);
	D3DXVECTOR3 lookAt(0.f, 0.f, 0.f);
	D3DXVECTOR3 up(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&matView, &eyePos, &lookAt, &up);

	//뷰행렬 셋팅
	Device->SetTransform(D3DTS_VIEW, &matView);

	//투영행렬 준비
	D3DXMATRIXA16 matProjection;

	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		60.f * ONE_RAD,
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),
		0.01f,
		100.f);

	//투영 행렬 셋팅
	Device->SetTransform(D3DTS_PROJECTION, &matProjection);

	Device->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void cScene_00::Release()
{
}

void cScene_00::Update(float timeDelta)
{
}

void cScene_00::Render()
{
	GIZMO_MGR->Circle(D3DXVECTOR3(0, 3, 0), 3.f, D3DXVECTOR3(1, 0, 0), 0xff00ffff);

	GIZMO_MGR->WireSphere(D3DXVECTOR3(-3, 0, 0), 2.f, 0xffffff00);

	GIZMO_MGR->AABBBox(D3DXVECTOR3(0, 1, -5), D3DXVECTOR3(2, -2, 8), 0xffffffff);

	GIZMO_MGR->WorldGrid(5, 10);
}