#pragma once

#include "cScene.h"

class cScene_03 : public cScene
{
private:

	//�����ϳ��� ���� ������ �����ϴ� ����ü
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//������ ��ġ
		DWORD		color;			//������ �÷�

		//���� ������ ������ ��Ÿ���� �÷��� ����� 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ ������ ��ġ���� | D3DFVF_DIFFUSE ������ �÷�����
	}MYVERTEX, *LPMYVERTEX;


	MYVERTEX		vertex[4];
	DWORD			index[6];
	float yaw;
	D3DXMATRIXA16	matWorld;

public:
	cScene_03();
	~cScene_03();

	void Init();
	void Release();
	void Update(float timeDelta);
	void Render();
};

