#include "stdafx.h"
#include "cScene_03.h"


cScene_03::cScene_03()
{
}


cScene_03::~cScene_03()
{
}


void cScene_03::Init()
{
	// 0------1
	// |	  |
	// |	  |
	// |	  |
	// 3------2

	vertex[0].pos = D3DXVECTOR3(-0.5f,  0.5f, 0.0f);
	vertex[1].pos = D3DXVECTOR3( 0.5f,  0.5f, 0.0f);
	vertex[2].pos = D3DXVECTOR3( 0.5f, -0.5f, 0.0f);
	vertex[3].pos = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);

	vertex[0].color = 0xffffff00;
	vertex[1].color = 0xffffff00;
	vertex[2].color = 0xfff00f00;
	vertex[3].color = 0xfff00f00;

	// 0------1
	// |	  |
	// |	  |
	// |	  |
	// 3------2

	//인덱스 (삼각형 그릴 순서대로)
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;

	index[3] = 0;
	index[4] = 2;
	index[5] = 3;

	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	yaw = 0.f;

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void cScene_03::Release()
{

}

void cScene_03::Update(float timeDelta)
{
	//초당 90도 회전
	this->yaw += (360.f * ONE_RAD) * timeDelta;

	D3DXMATRIXA16 matRotate;
	//3DXMatrixRotationAxis(결과 받을 행렬, 어느축으로 돌릴거냐, 각도);
	D3DXMatrixRotationAxis(&matRotate, &D3DXVECTOR3(0, 1, 0), yaw);

	//월드 행렬
	matWorld = matRotate;
}

void cScene_03::Render()
{
	//뷰행렬 준비
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 lookAt(0, 0, 0);
	D3DXVECTOR3 eyePos(0, 1, -10);
	D3DXVECTOR3 up(0, 1, 0);

	//D3DXMatrixLookAtLH(연산후 받을 행렬, 눈의 위치, 바라볼 곳, 업);
	D3DXMatrixLookAtLH(&matView, &eyePos, &lookAt, &up);

	//그리는 녀석한데 뷰행렬이 누군지 알려주자
	Device->SetTransform(D3DTS_VIEW, &matView);

	//투영행렬 준비
	D3DXMATRIXA16 matProjection;

	D3DXMatrixPerspectiveFovLH(
		&matProjection,		//결과 받을 투영행렬
		60.f * ONE_RAD,		//화각 (시야)
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),//종횡비
		0.001f,				//최소 시야 거리
		1000.f);			//최대 시야 거리

	//그리는 녀석한데 뷰행렬이 누군지 알려주자
	Device->SetTransform(D3DTS_PROJECTION, &matProjection);

	//월드 행렬 셋팅
	Device->SetTransform(D3DTS_WORLD, &matWorld);

	Device->SetFVF(MYVERTEX::FVF);

	Device->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,	//그리는 것은 삼각형이다
		0,					//최소 인덱스 시작점
		4,					//정점 갯수
		2,					//삼각형 갯수
		this->index,		//DWORD 인덱스 배열
		D3DFMT_INDEX32,		//인덱스 포멧 32비트 (만약 WORD 면, D3DFMT_INDEX16)
		this->vertex,		//정점 배열
		sizeof(MYVERTEX));	//정점 구조체 크기
}