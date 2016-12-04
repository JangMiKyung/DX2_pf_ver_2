#pragma once
#include "cScene.h"

class cScene_06 : public cScene
{
private:

	//�����ϳ��� ���� ������ �����ϴ� ����ü
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//������ ��ġ
		DWORD		color;			//������ �÷�

		//���� ������ ������ ��Ÿ���� �÷��� ����� 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ ������ ��ġ���� | D3DFVF_DIFFUSE ������ �÷�����
	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9			vb;			//������ ������ ���� ����
	LPDIRECT3DINDEXBUFFER9			ib;			//���� �ε��������� ������ �ε��� ����

	D3DXMATRIXA16					matWorld;	//���� ���

	float							yaw;

public:
	cScene_06(void);
	~cScene_06(void);

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

};
