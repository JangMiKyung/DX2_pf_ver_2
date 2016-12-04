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
	//큐브 생성
	this->CreateCube();
	//									   0xAARRGGBB
	this->cubeMaterial.Diffuse = D3DXCOLOR(0xffffff00);
	this->cubeMaterial.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	this->cubeMaterial.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	this->cubeMaterial.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);//셋팅된 Amient 를 정직하게 받겟다...
	this->cubeMaterial.Power = 10.0f;


	//라이팅
	ZeroMemory(&this->light, sizeof(D3DLIGHT9));
	this->light.Type = D3DLIGHT_DIRECTIONAL;						//방향성 광원 타입
	this->light.Direction = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);		//방향성 광원 방향
	this->light.Diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);		//광원 Diffuse 컬러
	this->light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//광원 Specular 컬러

	//라이팅 셋팅
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);		//광원 사용하겠음....

	Device->SetLight(0, &this->light);			//0 번 광원 정보 셋팅
	Device->LightEnable(0, TRUE);				//0 번 광원 활성화....

	//Ambient 셑팅
	Device->SetRenderState(D3DRS_AMBIENT, 0xff000000);

	//스펙큘러 ( 정반사 활성화 )
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

}

void cScene_00::Release()
{
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_00::Update(float timeDelta)
{
	float deltaMove = 3.0f * timeDelta;
	float deltaAngle = 90.0f * ONE_RAD * timeDelta;

	if (KEY_MGR->IsStayDown(VK_RBUTTON) == false)
	{
		if (KEY_MGR->IsStayDown('A'))
			this->lightTrans.MovePositionSelf(-deltaMove, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown('D'))
			this->lightTrans.MovePositionSelf(deltaMove, 0.0f, 0.0f);

		if (KEY_MGR->IsStayDown('W'))
			this->lightTrans.MovePositionSelf(0.0f, 0.0f, deltaMove);
		else if (KEY_MGR->IsStayDown('S'))
			this->lightTrans.MovePositionSelf(0.0f, 0.0f, -deltaMove);

		if (KEY_MGR->IsStayDown(VK_LEFT))
			this->lightTrans.RotateSelf(0.0f, -deltaAngle, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_RIGHT))
			this->lightTrans.RotateSelf(0.0f, deltaAngle, 0.0f);

		if (KEY_MGR->IsStayDown(VK_UP))
			this->lightTrans.RotateSelf(-deltaAngle, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_DOWN))
			this->lightTrans.RotateSelf(deltaAngle, 0.0f, 0.0f);
	}

	//라이팅 방향 갱신
	this->light.Direction = this->lightTrans.GetForward(); //갱신된 라이팅 다시 셋팅
	Device->SetLight(0, &this->light);	//0 번 광원 정보 셋팅


	//카메라 Default 컨트롤
	mainCamera.DefaultControl(timeDelta);

	//카메라 업데이트
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_00::Render()
{
	//큐브의 재질셋팅 ( 중요 ************* 한번랜더링명령이 실행될때 셋팅할수 있는 메터리얼수는 1 개이다 )
	Device->SetMaterial(&cubeMaterial);

	this->cubeTrans.SetDeviceWorld(Device);
	this->RenderCube();

	lightTrans.RenderGimozo();
}

void cScene_00::CreateCube()
{
	//
	// 정점 버퍼 생성
	//
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 24,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,		//저장될 메모리 풀 설정
		&this->vb,
		NULL);

	//저장될 메모리풀
	//D3DPOOL_DEFAULT			//정점버퍼의 메모리는 시스템사양에 맞춰 알아서 저장이되는데 보통은 비디오 카드메모리에 저장이된다.
	//D3DPOOL_MANAGED			//정점버퍼의 메모리는 시스템의 메모리영역에 저장이되고 비디오카드 메모리를 사용한다면 비디오카드 메모리에도 저장이된다, 즉 버퍼의 원본내용이 System 메모리에 저장이된 상태에서 비디오카드 메모리에 그거와 똑같은 사본을 만들어쓰는 방식인데... 이는 Device 가 해제 되어도 버퍼의 내용을 다시 복구할 수 있는 여지가 있다.


	//쿼드 플랜 정점
	MYVERTEX quad[4];
	quad[0].pos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	quad[1].pos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	quad[2].pos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	quad[3].pos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);


	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, -90.0f * ONE_RAD);


	//정점 배열준비
	MYVERTEX vertices[24];

	//앞면 
	memcpy(vertices, quad, sizeof(MYVERTEX) * 4);
	vertices[0].normal = D3DXVECTOR3(0, 0, -1);
	vertices[1].normal = D3DXVECTOR3(0, 0, -1);
	vertices[2].normal = D3DXVECTOR3(0, 0, -1);
	vertices[3].normal = D3DXVECTOR3(0, 0, -1);


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//우측면 복사
	memcpy(vertices + 4, quad, sizeof(MYVERTEX) * 4);
	vertices[4].normal = D3DXVECTOR3(1, 0, 0);
	vertices[5].normal = D3DXVECTOR3(1, 0, 0);
	vertices[6].normal = D3DXVECTOR3(1, 0, 0);
	vertices[7].normal = D3DXVECTOR3(1, 0, 0);


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//뒷면 복사
	memcpy(vertices + 8, quad, sizeof(MYVERTEX) * 4);
	vertices[8].normal = D3DXVECTOR3(0, 0, 1);
	vertices[9].normal = D3DXVECTOR3(0, 0, 1);
	vertices[10].normal = D3DXVECTOR3(0, 0, 1);
	vertices[11].normal = D3DXVECTOR3(0, 0, 1);


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//좌측면 복사
	memcpy(vertices + 12, quad, sizeof(MYVERTEX) * 4);
	vertices[12].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[13].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[14].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[15].normal = D3DXVECTOR3(-1, 0, 0);



	D3DXMatrixRotationZ(&matRot, -90.0f * ONE_RAD);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//윗면 복사
	memcpy(vertices + 16, quad, sizeof(MYVERTEX) * 4);
	vertices[16].normal = D3DXVECTOR3(0, 1, 0);
	vertices[17].normal = D3DXVECTOR3(0, 1, 0);
	vertices[18].normal = D3DXVECTOR3(0, 1, 0);
	vertices[19].normal = D3DXVECTOR3(0, 1, 0);


	D3DXMatrixRotationZ(&matRot, -180.0f * ONE_RAD);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//아랫면 복사
	memcpy(vertices + 20, quad, sizeof(MYVERTEX) * 4);
	vertices[20].normal = D3DXVECTOR3(0, -1, 0);
	vertices[21].normal = D3DXVECTOR3(0, -1, 0);
	vertices[22].normal = D3DXVECTOR3(0, -1, 0);
	vertices[23].normal = D3DXVECTOR3(0, -1, 0);


	//버퍼 Lock
	void* pVertices = NULL;
	this->vb->Lock(0, 0, &pVertices, 0);

	//얻어온 포인터에 정점정보 메모리 복사 
	memcpy(pVertices, vertices, sizeof(MYVERTEX) * 24);

	this->vb->Unlock();



	//
	// 인덱스 버퍼 생성
	//

	Device->CreateIndexBuffer(
		sizeof(WORD) * 36,		//Cube 는 36 개의 인덱스 정보가 필요,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,				//WORD 형인덱스는 2byte 이니 16 비트인덱스이다, DWORD 로 하면 4byte 니 32 비트 인덱스가 되어야 한다.
		D3DPOOL_DEFAULT,
		&this->ib,					//얻어올 인덱스버퍼 포인터
		NULL);

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//인덱스 정보
	WORD indices[36];

	for (int i = 0; i < 6; i++)
	{
		//정점 스타트 인덱스
		WORD vertexIndex = i * 4;

		//인덱스 배열 스타트 인덱스
		WORD indicexIndex = i * 6;

		indices[indicexIndex + 0] = vertexIndex + 0;
		indices[indicexIndex + 1] = vertexIndex + 1;
		indices[indicexIndex + 2] = vertexIndex + 2;
		indices[indicexIndex + 3] = vertexIndex + 0;
		indices[indicexIndex + 4] = vertexIndex + 2;
		indices[indicexIndex + 5] = vertexIndex + 3;
	}

	//익덱스벙보 Lock
	void* pIndices = NULL;
	this->ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, indices, sizeof(WORD) * 36);

	this->ib->Unlock();
}

void cScene_00::RenderCube()
{
	//스트림소스 셋팅
	Device->SetStreamSource(0, this->vb, 0, sizeof(MYVERTEX));

	//인덱스 버퍼 셋팅
	Device->SetIndices(this->ib);

	//지금그리는 정점에 대한 정점 정보 FVF 플레그 값을 넣어준다. 
	//Device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	Device->SetFVF(MYVERTEX::FVF);

	//SetStreamSource 셋팅된 정점버퍼와 인덱스 버의 내용으로 그린다.
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,		//시작 버텍스배열 인덱스 
		0,
		24,		//정점 갯수 
		0,		//시작 인덱스배열 인덱스
		12		//그리는 개채수 삼각형이니깐 12 개
		);
}