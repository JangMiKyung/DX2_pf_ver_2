#pragma once

#include "cScene.h"

//FVF??

//FVF�� flexible Vertex Format �� ���ڷ�
//D3D ���� ������ �ִ� ���� ���� �߿��� ����ڰ� 
//���Ƿ� ������ ���ο� ���� ������ ���ϴ� ����̴�.

//1. ������ ������ ��ǥ x,y,z (float)  : D3DFVF_XYZ
//2. RHW(float)						: D3DFVF_XYZRHW
//3. Blending Weight Data  ���� ����ġ (float)	: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
//4. Vertex normal ������ ���� ����(x,y,z)(float) : D3DFVF_NORMAL
//5. Vertex Point Size ������ �� ũ�� (float)    : D3DFVF_PSIZE
//6. Diffuse Color (DWORD)                     : D3DFVF_DIFFUSE
//7. Specular Color (DWORD)                    : D3DFVF_SPECULAR
//8. Texture Coordinate Set 1 (float)          : D3DFVF_TEX0 - D3DFVF_TEX8

class cScene_00 : public cScene
{
private:
	//�����ϳ��� ���� ������ �����ϴ� ����ü
	typedef struct tagMYVERTEX {
		D3DXVECTOR3 pos;			//������ ��ġ
		DWORD		color;			//������ �÷�

									//���� ������ ������ ��Ÿ���� �÷��� ����� 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ ������ ��ġ���� | D3DFVF_DIFFUSE ������ �÷�����

													// FVF �� ���� ������ ���� ����ü �ڷ����� ������� ���־�� �Ѵ�.
	}MYVERTEX, *LPMYVERTEX;

	MYVERTEX			vertices[3];			//���� ����ü 3��	


public:
	cScene_00();
	~cScene_00();

	virtual void Init();

	virtual void Release();

	virtual void Update( float timeDelta );

	virtual void Render();
};

