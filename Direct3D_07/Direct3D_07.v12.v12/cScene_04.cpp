#include "stdafx.h"
#include "cScene_04.h"


cScene_04::cScene_04()
{
}


cScene_04::~cScene_04()
{
}

void cScene_04::Init()
{
	//큐브 생성
	this->CreateCube();

	//
	// 본 조립
	//

	//Root 본 생성
	this->BoneMap.insert(std::make_pair("Pivot", new cTransform()));
	this->BoneMap.insert(std::make_pair("Root", new cTransform()));

	//궁뎅이
	this->BoneMap.insert(std::make_pair("LeftHip", new cTransform()));
	this->BoneMap.insert(std::make_pair("RightHip", new cTransform()));

	//허벅지
	this->BoneMap.insert(std::make_pair("LeftUpLeg", new cTransform()));
	this->BoneMap.insert(std::make_pair("RightUpLeg", new cTransform()));

	//종아리
	this->BoneMap.insert(std::make_pair("LeftDownLeg", new cTransform()));
	this->BoneMap.insert(std::make_pair("RightDownLeg", new cTransform()));
	
	//발
	this->BoneMap.insert(std::make_pair("LeftFoot", new cTransform()));
	this->BoneMap.insert(std::make_pair("RightFoot", new cTransform()));

	//조립
	this->BoneMap["Root"]->AttachTo(this->BoneMap["Pivot"]);
	this->BoneMap["LeftHip"]->AttachTo(this->BoneMap["Root"]);
	this->BoneMap["RightHip"]->AttachTo(this->BoneMap["Root"]);
	this->BoneMap["LeftHip"]->SetLocalPosition(-1.0f, 0.0f, 0.0f);
	this->BoneMap["RightHip"]->SetLocalPosition(1.0f, 0.0f, 0.0f);

	this->BoneMap["LeftUpLeg"]->AttachTo(this->BoneMap["LeftHip"]);
	this->BoneMap["RightUpLeg"]->AttachTo(this->BoneMap["RightHip"]);
	this->BoneMap["LeftUpLeg"]->SetLocalPosition(0.0f, -0.5f, 0.0f);
	this->BoneMap["RightUpLeg"]->SetLocalPosition(0.0f, -0.5f, 0.0f);

	this->BoneMap["LeftDownLeg"]->AttachTo(this->BoneMap["LeftUpLeg"]);
	this->BoneMap["RightDownLeg"]->AttachTo(this->BoneMap["RightUpLeg"]);
	this->BoneMap["LeftDownLeg"]->SetLocalPosition(0.0f, -2.0f, 0.0f);
	this->BoneMap["RightDownLeg"]->SetLocalPosition(0.0f, -2.0f, 0.0f);

	this->BoneMap["LeftFoot"]->AttachTo(this->BoneMap["LeftDownLeg"]);
	this->BoneMap["RightFoot"]->AttachTo(this->BoneMap["RightDownLeg"]);
	this->BoneMap["LeftFoot"]->SetLocalPosition(0.0f, -2.0f, 0.0f);
	this->BoneMap["RightFoot"]->SetLocalPosition(0.0f, -2.0f, 0.0f);

	//
	// 본 붙는 메쉬
	//
	//Root 메쉬
	this->MeshMap.insert(std::make_pair("Root", new cTransform()));
	this->MeshMap["Root"]->AttachTo(this->BoneMap["Root"]);

	this->MeshMap.insert(std::make_pair("LeftHip", new cTransform()));
	this->MeshMap["LeftHip"]->AttachTo(this->BoneMap["LeftHip"]);
	this->MeshMap["LeftHip"]->Reset();

	this->MeshMap.insert(std::make_pair("RightHip", new cTransform()));
	this->MeshMap["RightHip"]->AttachTo(this->BoneMap["RightHip"]);
	this->MeshMap["RightHip"]->Reset();

	this->MeshMap.insert(std::make_pair("LeftUpLeg", new cTransform()));
	this->MeshMap["LeftUpLeg"]->AttachTo(this->BoneMap["LeftUpLeg"]);
	this->MeshMap["LeftUpLeg"]->Reset();
	this->MeshMap["LeftUpLeg"]->SetScale(1, 2, 1);
	this->MeshMap["LeftUpLeg"]->SetLocalPosition(0, -1, 0);

	this->MeshMap.insert(std::make_pair("RightUpLeg", new cTransform()));
	this->MeshMap["RightUpLeg"]->AttachTo(this->BoneMap["RightUpLeg"]);
	this->MeshMap["RightUpLeg"]->Reset();
	this->MeshMap["RightUpLeg"]->SetScale(1, 2, 1);
	this->MeshMap["RightUpLeg"]->SetLocalPosition(0, -1, 0);

	this->MeshMap.insert(std::make_pair("LeftDownLeg", new cTransform()));
	this->MeshMap["LeftDownLeg"]->AttachTo(this->BoneMap["LeftDownLeg"]);
	this->MeshMap["LeftDownLeg"]->Reset();
	this->MeshMap["LeftDownLeg"]->SetScale(1, 2, 1);
	this->MeshMap["LeftDownLeg"]->SetLocalPosition(0, -1, 0);

	this->MeshMap.insert(std::make_pair("RightDownLeg", new cTransform()));
	this->MeshMap["RightDownLeg"]->AttachTo(this->BoneMap["RightDownLeg"]);
	this->MeshMap["RightDownLeg"]->Reset();
	this->MeshMap["RightDownLeg"]->SetScale(1, 2, 1);
	this->MeshMap["RightDownLeg"]->SetLocalPosition(0, -1, 0);

	this->MeshMap.insert(std::make_pair("LeftFoot", new cTransform()));
	this->MeshMap["LeftFoot"]->AttachTo(this->BoneMap["LeftFoot"]);
	this->MeshMap["LeftFoot"]->Reset();
	this->MeshMap["LeftFoot"]->SetScale(1, 0.5f, 1.5f);
	this->MeshMap["LeftFoot"]->SetLocalPosition(0, -0.25f, 0.25f);

	this->MeshMap.insert(std::make_pair("RightFoot", new cTransform()));
	this->MeshMap["RightFoot"]->AttachTo(this->BoneMap["RightFoot"]);
	this->MeshMap["RightFoot"]->Reset();
	this->MeshMap["RightFoot"]->SetScale(1, 0.5f, 1.5f);
	this->MeshMap["RightFoot"]->SetLocalPosition(0, -0.25f, 0.25f);

	//
	// 키프레임 생성 ( 초당 10 프레임이 증가 된다고 가정 )
	//

	//루트도 추가
	KEY_FRAMES rootHipKeys;

	this->BoneMap["Root"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rootHipKeys.push_back(new KEY_FRAME(0, *this->BoneMap["Root"]));		// 0 프레임 넣는다.

	this->BoneMap["Root"]->SetRotateLocal(0.0f, 30.0f * ONE_RAD, 0.0f);
	rootHipKeys.push_back(new KEY_FRAME(15, *this->BoneMap["Root"]));

	this->BoneMap["Root"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rootHipKeys.push_back(new KEY_FRAME(30, *this->BoneMap["Root"]));		// 30 프레임 넣는다.

	//맵에 추가
	this->animationFrames.insert(std::make_pair("Root", rootHipKeys));

	//오른쪽 허벅지 추가
	KEY_FRAMES rightHipKeys;

	this->BoneMap["RightHip"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rightHipKeys.push_back(new KEY_FRAME(0, *this->BoneMap["RightHip"]));		// 0 프레임 넣는다.

	this->BoneMap["RightHip"]->SetRotateLocal(-120.0f * ONE_RAD, 0.0f, 0.0f);
	rightHipKeys.push_back(new KEY_FRAME(10, *this->BoneMap["RightHip"]));

	this->BoneMap["RightHip"]->SetRotateLocal(-120.0f * ONE_RAD, 0.0f, 90.0f * ONE_RAD);
	rightHipKeys.push_back(new KEY_FRAME(20, *this->BoneMap["RightHip"]));

	this->BoneMap["RightHip"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rightHipKeys.push_back(new KEY_FRAME(30, *this->BoneMap["RightHip"]));		// 30 프레임 넣는다.

	//맵에 추가
	this->animationFrames.insert(std::make_pair("RightHip", rightHipKeys));

	//오른쪽 다리추가
	KEY_FRAMES rightDownLegKeys;

	this->BoneMap["RightDownLeg"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rightDownLegKeys.push_back(new KEY_FRAME(0, *this->BoneMap["RightDownLeg"]));		// 0 프레임 넣는다.

	this->BoneMap["RightDownLeg"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rightDownLegKeys.push_back(new KEY_FRAME(5, *this->BoneMap["RightDownLeg"]));

	this->BoneMap["RightDownLeg"]->SetRotateLocal(60.0f * ONE_RAD, 0.0f, 0.0f);
	rightDownLegKeys.push_back(new KEY_FRAME(10, *this->BoneMap["RightDownLeg"]));

	this->BoneMap["RightDownLeg"]->SetRotateLocal(180.0f * ONE_RAD, 0.0f, 0.0f);
	rightDownLegKeys.push_back(new KEY_FRAME(20, *this->BoneMap["RightDownLeg"]));

	this->BoneMap["RightDownLeg"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rightDownLegKeys.push_back(new KEY_FRAME(30, *this->BoneMap["RightDownLeg"]));		// 30 프레임 넣는다.

	//맵에 추가
	this->animationFrames.insert(std::make_pair("RightDownLeg", rightDownLegKeys));

	//본자동 업데이트 막는다.
	for (MAP_TRANS_ITER iter = this->BoneMap.begin();
		iter != this->BoneMap.end();
		++iter)
	{
		iter->second->SetAutoUpdate(false);
	}
	this->Trans.SetAutoUpdate(false);

	nowFrame = 0.0f;

	//Trans 에 Pivot 본 붙인다.
	this->Trans.AddChild(this->BoneMap["Pivot"]);
	this->BoneMap["Pivot"]->SetLocalPosition(0, 5, 0);

	//정점의 컬러 정보를 출력하기 위해서는 Device 의 기본 라이팅 
	//기능을 해재해야 한다
	//당분간 라이팅을 사용할일이 없으니 라이팅을 끄자.
	Device->SetRenderState(
		D3DRS_LIGHTING,			//뭘셋팅할꺼니?
		FALSE					//셋팅값은 어떻게 되니?
		);
}


void cScene_04::Release()
{
	for (MAP_TRANS_ITER iter = this->BoneMap.begin();
		iter != this->BoneMap.end();
		++iter)
	{
		SAFE_DELETE(iter->second);
	}

	this->BoneMap.clear();

	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}


void cScene_04::Update(float timeDelta)
{
	float deltaMove = 3.0f * timeDelta;
	float deltaAngle = 90.0f * ONE_RAD * timeDelta;



	static float angle = 0.0f;
	angle += 360.0f * ONE_RAD * timeDelta;

	this->BoneMap["Pivot"]->SetLocalPosition(0, 5 + sin(angle) * 0.5f, 0);

	//프레임 증가
	nowFrame += timeDelta * 30.0f;
	if (nowFrame > 30.0f)
		nowFrame -= 30.0f;

	//Animation 처리
	for (MAP_BONE_KEYFRAMES::iterator iter = this->animationFrames.begin();
		iter != this->animationFrames.end();
		++iter)
	{
		KEY_FRAMES keyframes = iter->second;


		int frontIndex = 0; // 0 
		int backIndex = keyframes.size() - 1;
		int centerIndex = (backIndex - frontIndex) / 2 + frontIndex;

		while (frontIndex < backIndex){

			//센터인덱스의 프레임 값이 현제 프레임보다 크니?
			if (keyframes[centerIndex]->frame >= nowFrame){

				backIndex = centerIndex;
				//센터 갱신
				centerIndex = (backIndex - frontIndex) / 2 + frontIndex;
			}

			//센터인덱스의 프레임 값이 현제 프레임보다 작니?
			else
			{

				frontIndex = centerIndex;
				//센터 갱신
				centerIndex = (backIndex - frontIndex) / 2 + frontIndex;
			}

			if (backIndex == frontIndex + 1){
				break;
			}
		}

		//보간해야할 앞인덱스와 뒤쪽 인덱스를 찾았다...

		//현제의 프레임이 frontIndex 와 backIndex 사이에서 얼만큼 진행되었는가를 0 ~ 1 로 얻는다.
		float t = InverseLerp(
			keyframes[frontIndex]->frame,
			keyframes[backIndex]->frame,
			this->nowFrame);

		//보간 시킨다.
		this->BoneMap[iter->first]->Interpolate(
			keyframes[frontIndex]->trans,
			keyframes[backIndex]->trans,
			t);


	}

	if (KEY_MGR->IsStayDown(VK_RBUTTON) == false)
	{
		if (KEY_MGR->IsStayDown('A'))
			this->Trans.MovePositionSelf(-deltaMove, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown('D'))
			this->Trans.MovePositionSelf(deltaMove, 0.0f, 0.0f);

		if (KEY_MGR->IsStayDown('W'))
			this->Trans.MovePositionSelf(0.0f, 0.0f, deltaMove);
		else if (KEY_MGR->IsStayDown('S'))
			this->Trans.MovePositionSelf(0.0f, 0.0f, -deltaMove);

		if (KEY_MGR->IsStayDown(VK_LEFT))
			this->Trans.RotateSelf(0.0f, -deltaAngle, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_RIGHT))
			this->Trans.RotateSelf(0.0f, deltaAngle, 0.0f);

		if (KEY_MGR->IsStayDown(VK_UP))
			this->Trans.RotateSelf(-deltaAngle, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_DOWN))
			this->Trans.RotateSelf(deltaAngle, 0.0f, 0.0f);
	}


	Trans.UpdateTransform();

	//카메라 Default 컨트롤
	mainCamera.DefaultControl(timeDelta);

	//카메라 업데이트
	mainCamera.UpdateCamToDevice(Device);
}


void cScene_04::Render()
{
	for (MAP_TRANS_ITER iter = this->BoneMap.begin();
		iter != this->BoneMap.end();
		++iter)
	{
		iter->second->RenderGimozo();
	}

	for (MAP_TRANS_ITER iter = this->MeshMap.begin();
		iter != this->MeshMap.end();
		++iter)
	{
		iter->second->SetDeviceWorld(Device);
		RenderCube();
	}

	this->Trans.RenderGimozo(Device);
}




void cScene_04::CreateCube()
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
	vertices[0].color = 0xff000080;
	vertices[1].color = 0xff000080;
	vertices[2].color = 0xff000080;
	vertices[3].color = 0xff000080;


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//우측면 복사
	memcpy(vertices + 4, quad, sizeof(MYVERTEX) * 4);
	vertices[4].color = 0xffff0000;
	vertices[5].color = 0xffff0000;
	vertices[6].color = 0xffff0000;
	vertices[7].color = 0xffff0000;


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//뒷면 복사
	memcpy(vertices + 8, quad, sizeof(MYVERTEX) * 4);
	vertices[8].color = 0xff0000ff;
	vertices[9].color = 0xff0000ff;
	vertices[10].color = 0xff0000ff;
	vertices[11].color = 0xff0000ff;


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//좌측면 복사
	memcpy(vertices + 12, quad, sizeof(MYVERTEX) * 4);
	vertices[12].color = 0xff800000;
	vertices[13].color = 0xff800000;
	vertices[14].color = 0xff800000;
	vertices[15].color = 0xff800000;



	D3DXMatrixRotationZ(&matRot, -90.0f * ONE_RAD);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//윗면 복사
	memcpy(vertices + 16, quad, sizeof(MYVERTEX) * 4);
	vertices[16].color = 0xff00ff00;
	vertices[17].color = 0xff00ff00;
	vertices[18].color = 0xff00ff00;
	vertices[19].color = 0xff00ff00;


	D3DXMatrixRotationZ(&matRot, -180.0f * ONE_RAD);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//아랫면 복사
	memcpy(vertices + 20, quad, sizeof(MYVERTEX) * 4);
	vertices[20].color = 0xff008000;
	vertices[21].color = 0xff008000;
	vertices[22].color = 0xff008000;
	vertices[23].color = 0xff008000;


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


void cScene_04::RenderCube()
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