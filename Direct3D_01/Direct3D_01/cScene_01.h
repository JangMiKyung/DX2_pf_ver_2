#pragma once

#include "cScene.h"

class cScene_01 : public cScene
{
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//������ ��ġ
		DWORD		color;			//������ �÷�

		//���� ������ ������ ��Ÿ���� �÷��� ����� 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ ������ ��ġ���� | D3DFVF_DIFFUSE ������ �÷�����
	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9				planeVB;			//����� ���� ����]

	float								yawSpeed;			//�ʴ� ���� ȸ�� �ӵ�
	float								yawAngle;			//���� ��
	float								moveSpeed;			//�ʴ� �̵���
	D3DXVECTOR3							pos;				//����� ��ġ
	D3DXMATRIXA16						matPlane;			//����� ���


	//�ڽ� ����� ����
	D3DXVECTOR3							localPos;			//�θ�������� ��� �ִ�?
	D3DXMATRIXA16						matChildPlane;		//�ڽĺ���� ���

public:
	cScene_01();
	~cScene_01();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

};

