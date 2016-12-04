#pragma once

#include "cScene.h"

class cScene_02 : public cScene
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

	//�繰�� �� ( ��� ���� ���⿡ ���� ������ �����Ѵ� )
	D3DXVECTOR3						forward;	//�繰�� ���� ����
	D3DXVECTOR3						right;		//�繰�� ������ ����
	D3DXVECTOR3						up;			//�繰�� ��溤��

public:
	cScene_02();
	~cScene_02();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:

	void CreateCube();
	
	void RotateSelfYaw(float angle);
	void RotateSelfPitch(float angle);
	void RotateSelfRoll(float angle);

	void RotateWorldYaw(float angle);
	void RotateWorldPitch(float angle);
	void RotateWorldRoll(float angle);

};

