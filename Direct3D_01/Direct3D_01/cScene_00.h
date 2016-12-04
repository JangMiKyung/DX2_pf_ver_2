#pragma once

#include "cScene.h"

class cScene_00 : public cScene
{
private:
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//������ ��ġ
		DWORD		color;			//������ �÷�

		//���� ������ ������ ��Ÿ���� �÷��� ����� 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ ������ ��ġ���� | D3DFVF_DIFFUSE ������ �÷�����
	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9 planeVB; //����� ���� ����

	float yawSpeed;
	float yawAngle;
	float moveSpeed;
	D3DXVECTOR3 pos;
	D3DXMATRIXA16 matPlane;

	D3DXVECTOR3 pos2;
	D3DXMATRIXA16 matPlane2;


public:
	cScene_00();
	virtual ~cScene_00();

	virtual void Init();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();
};

