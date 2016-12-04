#pragma once
#include "cScene.h"
class cScene_01 : public cScene
{
private:
	//�����ϳ��� ���� ������ �����ϴ� ����ü
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//������ ��ġ
		D3DXVECTOR3 normal;			//������ �븻���� ( ���� ǥ���� �ٶ󺸴� ������ ��´� )

		//���� ������ ������ ��Ÿ���� �÷��� ����� 
		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };	//D3DFVF_XYZ ������ ��ġ���� | D3DFVF_NORMAL ������ �븻����

	}MYVERTEX, *LPMYVERTEX;


	LPDIRECT3DVERTEXBUFFER9			vb;				//������ ������ ���� ����
	LPDIRECT3DINDEXBUFFER9			ib;				//���� �ε��������� ������ �ε��� ����

	cTransform						cubeTrans;
	cCamera							mainCamera;
	D3DMATERIAL9					cubeMaterial;		//ť�� ����

	D3DLIGHT9						light;				//���� ����
	cTransform						lightTrans;			//���� Transform

public:
	cScene_01();
	~cScene_01();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:

	void CreateCubeHardEdge();
	void CreateCubeSoftEdge();


	void RenderCube();
};

