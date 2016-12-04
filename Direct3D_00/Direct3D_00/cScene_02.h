#pragma once

#include "cScene.h"

class cScene_02 : public cScene
{
private:
	typedef struct tagMYVERTEX
	{
		D3DXVECTOR3 pos;
		DWORD		color;

		enum{ FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	}MYVERTEX, *LPMYVERTEX;

	//���� �����͸� ������ ���ؽ����� ������
	LPDIRECT3DVERTEXBUFFER9 vb;
	float yaw; //y�� ȸ��
	D3DXMATRIXA16 matWorld;//���� ���

public:
	cScene_02();
	~cScene_02();

	virtual void Init();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();
};

