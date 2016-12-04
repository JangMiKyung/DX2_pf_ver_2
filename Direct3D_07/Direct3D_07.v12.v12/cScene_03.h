#pragma once
#include "cScene.h"
#include <vector>

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


	LPDIRECT3DVERTEXBUFFER9			vb;				//������ ������ ���� ����
	LPDIRECT3DINDEXBUFFER9			ib;				//���� �ε��������� ������ �ε��� ����

	cTransform						cubeTrans;		//ť���� Transform
	std::vector<cTransform*>		followTrans;	//����ٴϴ� Transform ��.....

	cTransform						camTrans;		//ī�޶� Transform
	cCamera							mainCamera;

public:
	cScene_03();
	~cScene_03();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:

	void CreateCube();

	void RenderCube();
};
