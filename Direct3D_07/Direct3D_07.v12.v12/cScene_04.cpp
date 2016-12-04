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
	//ť�� ����
	this->CreateCube();

	//
	// �� ����
	//

	//Root �� ����
	this->BoneMap.insert(std::make_pair("Pivot", new cTransform()));
	this->BoneMap.insert(std::make_pair("Root", new cTransform()));

	//�õ���
	this->BoneMap.insert(std::make_pair("LeftHip", new cTransform()));
	this->BoneMap.insert(std::make_pair("RightHip", new cTransform()));

	//�����
	this->BoneMap.insert(std::make_pair("LeftUpLeg", new cTransform()));
	this->BoneMap.insert(std::make_pair("RightUpLeg", new cTransform()));

	//���Ƹ�
	this->BoneMap.insert(std::make_pair("LeftDownLeg", new cTransform()));
	this->BoneMap.insert(std::make_pair("RightDownLeg", new cTransform()));
	
	//��
	this->BoneMap.insert(std::make_pair("LeftFoot", new cTransform()));
	this->BoneMap.insert(std::make_pair("RightFoot", new cTransform()));

	//����
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
	// �� �ٴ� �޽�
	//
	//Root �޽�
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
	// Ű������ ���� ( �ʴ� 10 �������� ���� �ȴٰ� ���� )
	//

	//��Ʈ�� �߰�
	KEY_FRAMES rootHipKeys;

	this->BoneMap["Root"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rootHipKeys.push_back(new KEY_FRAME(0, *this->BoneMap["Root"]));		// 0 ������ �ִ´�.

	this->BoneMap["Root"]->SetRotateLocal(0.0f, 30.0f * ONE_RAD, 0.0f);
	rootHipKeys.push_back(new KEY_FRAME(15, *this->BoneMap["Root"]));

	this->BoneMap["Root"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rootHipKeys.push_back(new KEY_FRAME(30, *this->BoneMap["Root"]));		// 30 ������ �ִ´�.

	//�ʿ� �߰�
	this->animationFrames.insert(std::make_pair("Root", rootHipKeys));

	//������ ����� �߰�
	KEY_FRAMES rightHipKeys;

	this->BoneMap["RightHip"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rightHipKeys.push_back(new KEY_FRAME(0, *this->BoneMap["RightHip"]));		// 0 ������ �ִ´�.

	this->BoneMap["RightHip"]->SetRotateLocal(-120.0f * ONE_RAD, 0.0f, 0.0f);
	rightHipKeys.push_back(new KEY_FRAME(10, *this->BoneMap["RightHip"]));

	this->BoneMap["RightHip"]->SetRotateLocal(-120.0f * ONE_RAD, 0.0f, 90.0f * ONE_RAD);
	rightHipKeys.push_back(new KEY_FRAME(20, *this->BoneMap["RightHip"]));

	this->BoneMap["RightHip"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rightHipKeys.push_back(new KEY_FRAME(30, *this->BoneMap["RightHip"]));		// 30 ������ �ִ´�.

	//�ʿ� �߰�
	this->animationFrames.insert(std::make_pair("RightHip", rightHipKeys));

	//������ �ٸ��߰�
	KEY_FRAMES rightDownLegKeys;

	this->BoneMap["RightDownLeg"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rightDownLegKeys.push_back(new KEY_FRAME(0, *this->BoneMap["RightDownLeg"]));		// 0 ������ �ִ´�.

	this->BoneMap["RightDownLeg"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rightDownLegKeys.push_back(new KEY_FRAME(5, *this->BoneMap["RightDownLeg"]));

	this->BoneMap["RightDownLeg"]->SetRotateLocal(60.0f * ONE_RAD, 0.0f, 0.0f);
	rightDownLegKeys.push_back(new KEY_FRAME(10, *this->BoneMap["RightDownLeg"]));

	this->BoneMap["RightDownLeg"]->SetRotateLocal(180.0f * ONE_RAD, 0.0f, 0.0f);
	rightDownLegKeys.push_back(new KEY_FRAME(20, *this->BoneMap["RightDownLeg"]));

	this->BoneMap["RightDownLeg"]->SetRotateLocal(0.0f, 0.0f, 0.0f);
	rightDownLegKeys.push_back(new KEY_FRAME(30, *this->BoneMap["RightDownLeg"]));		// 30 ������ �ִ´�.

	//�ʿ� �߰�
	this->animationFrames.insert(std::make_pair("RightDownLeg", rightDownLegKeys));

	//���ڵ� ������Ʈ ���´�.
	for (MAP_TRANS_ITER iter = this->BoneMap.begin();
		iter != this->BoneMap.end();
		++iter)
	{
		iter->second->SetAutoUpdate(false);
	}
	this->Trans.SetAutoUpdate(false);

	nowFrame = 0.0f;

	//Trans �� Pivot �� ���δ�.
	this->Trans.AddChild(this->BoneMap["Pivot"]);
	this->BoneMap["Pivot"]->SetLocalPosition(0, 5, 0);

	//������ �÷� ������ ����ϱ� ���ؼ��� Device �� �⺻ ������ 
	//����� �����ؾ� �Ѵ�
	//��а� �������� ��������� ������ �������� ����.
	Device->SetRenderState(
		D3DRS_LIGHTING,			//�������Ҳ���?
		FALSE					//���ð��� ��� �Ǵ�?
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

	//������ ����
	nowFrame += timeDelta * 30.0f;
	if (nowFrame > 30.0f)
		nowFrame -= 30.0f;

	//Animation ó��
	for (MAP_BONE_KEYFRAMES::iterator iter = this->animationFrames.begin();
		iter != this->animationFrames.end();
		++iter)
	{
		KEY_FRAMES keyframes = iter->second;


		int frontIndex = 0; // 0 
		int backIndex = keyframes.size() - 1;
		int centerIndex = (backIndex - frontIndex) / 2 + frontIndex;

		while (frontIndex < backIndex){

			//�����ε����� ������ ���� ���� �����Ӻ��� ũ��?
			if (keyframes[centerIndex]->frame >= nowFrame){

				backIndex = centerIndex;
				//���� ����
				centerIndex = (backIndex - frontIndex) / 2 + frontIndex;
			}

			//�����ε����� ������ ���� ���� �����Ӻ��� �۴�?
			else
			{

				frontIndex = centerIndex;
				//���� ����
				centerIndex = (backIndex - frontIndex) / 2 + frontIndex;
			}

			if (backIndex == frontIndex + 1){
				break;
			}
		}

		//�����ؾ��� ���ε����� ���� �ε����� ã�Ҵ�...

		//������ �������� frontIndex �� backIndex ���̿��� ��ŭ ����Ǿ��°��� 0 ~ 1 �� ��´�.
		float t = InverseLerp(
			keyframes[frontIndex]->frame,
			keyframes[backIndex]->frame,
			this->nowFrame);

		//���� ��Ų��.
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

	//ī�޶� Default ��Ʈ��
	mainCamera.DefaultControl(timeDelta);

	//ī�޶� ������Ʈ
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
	// ���� ���� ����
	//
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 24,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,		//����� �޸� Ǯ ����
		&this->vb,
		NULL);

	//����� �޸�Ǯ
	//D3DPOOL_DEFAULT			//���������� �޸𸮴� �ý��ۻ�翡 ���� �˾Ƽ� �����̵Ǵµ� ������ ���� ī��޸𸮿� �����̵ȴ�.
	//D3DPOOL_MANAGED			//���������� �޸𸮴� �ý����� �޸𸮿����� �����̵ǰ� ����ī�� �޸𸮸� ����Ѵٸ� ����ī�� �޸𸮿��� �����̵ȴ�, �� ������ ���������� System �޸𸮿� �����̵� ���¿��� ����ī�� �޸𸮿� �װſ� �Ȱ��� �纻�� ������ ����ε�... �̴� Device �� ���� �Ǿ ������ ������ �ٽ� ������ �� �ִ� ������ �ִ�.


	//���� �÷� ����
	MYVERTEX quad[4];
	quad[0].pos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	quad[1].pos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	quad[2].pos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	quad[3].pos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);


	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, -90.0f * ONE_RAD);


	//���� �迭�غ�
	MYVERTEX vertices[24];

	//�ո� 
	memcpy(vertices, quad, sizeof(MYVERTEX) * 4);
	vertices[0].color = 0xff000080;
	vertices[1].color = 0xff000080;
	vertices[2].color = 0xff000080;
	vertices[3].color = 0xff000080;


	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//������ ����
	memcpy(vertices + 4, quad, sizeof(MYVERTEX) * 4);
	vertices[4].color = 0xffff0000;
	vertices[5].color = 0xffff0000;
	vertices[6].color = 0xffff0000;
	vertices[7].color = 0xffff0000;


	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//�޸� ����
	memcpy(vertices + 8, quad, sizeof(MYVERTEX) * 4);
	vertices[8].color = 0xff0000ff;
	vertices[9].color = 0xff0000ff;
	vertices[10].color = 0xff0000ff;
	vertices[11].color = 0xff0000ff;


	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//������ ����
	memcpy(vertices + 12, quad, sizeof(MYVERTEX) * 4);
	vertices[12].color = 0xff800000;
	vertices[13].color = 0xff800000;
	vertices[14].color = 0xff800000;
	vertices[15].color = 0xff800000;



	D3DXMatrixRotationZ(&matRot, -90.0f * ONE_RAD);

	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//���� ����
	memcpy(vertices + 16, quad, sizeof(MYVERTEX) * 4);
	vertices[16].color = 0xff00ff00;
	vertices[17].color = 0xff00ff00;
	vertices[18].color = 0xff00ff00;
	vertices[19].color = 0xff00ff00;


	D3DXMatrixRotationZ(&matRot, -180.0f * ONE_RAD);

	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//�Ʒ��� ����
	memcpy(vertices + 20, quad, sizeof(MYVERTEX) * 4);
	vertices[20].color = 0xff008000;
	vertices[21].color = 0xff008000;
	vertices[22].color = 0xff008000;
	vertices[23].color = 0xff008000;


	//���� Lock
	void* pVertices = NULL;
	this->vb->Lock(0, 0, &pVertices, 0);

	//���� �����Ϳ� �������� �޸� ���� 
	memcpy(pVertices, vertices, sizeof(MYVERTEX) * 24);

	this->vb->Unlock();



	//
	// �ε��� ���� ����
	//

	Device->CreateIndexBuffer(
		sizeof(WORD) * 36,		//Cube �� 36 ���� �ε��� ������ �ʿ�,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,				//WORD ���ε����� 2byte �̴� 16 ��Ʈ�ε����̴�, DWORD �� �ϸ� 4byte �� 32 ��Ʈ �ε����� �Ǿ�� �Ѵ�.
		D3DPOOL_DEFAULT,
		&this->ib,					//���� �ε������� ������
		NULL);

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//�ε��� ����
	WORD indices[36];

	for (int i = 0; i < 6; i++)
	{
		//���� ��ŸƮ �ε���
		WORD vertexIndex = i * 4;

		//�ε��� �迭 ��ŸƮ �ε���
		WORD indicexIndex = i * 6;

		indices[indicexIndex + 0] = vertexIndex + 0;
		indices[indicexIndex + 1] = vertexIndex + 1;
		indices[indicexIndex + 2] = vertexIndex + 2;
		indices[indicexIndex + 3] = vertexIndex + 0;
		indices[indicexIndex + 4] = vertexIndex + 2;
		indices[indicexIndex + 5] = vertexIndex + 3;
	}

	//�͵������� Lock
	void* pIndices = NULL;
	this->ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, indices, sizeof(WORD) * 36);

	this->ib->Unlock();
}


void cScene_04::RenderCube()
{
	//��Ʈ���ҽ� ����
	Device->SetStreamSource(0, this->vb, 0, sizeof(MYVERTEX));

	//�ε��� ���� ����
	Device->SetIndices(this->ib);

	//���ݱ׸��� ������ ���� ���� ���� FVF �÷��� ���� �־��ش�. 
	//Device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	Device->SetFVF(MYVERTEX::FVF);

	//SetStreamSource ���õ� �������ۿ� �ε��� ���� �������� �׸���.
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,		//���� ���ؽ��迭 �ε��� 
		0,
		24,		//���� ���� 
		0,		//���� �ε����迭 �ε���
		12		//�׸��� ��ä�� �ﰢ���̴ϱ� 12 ��
		);
}