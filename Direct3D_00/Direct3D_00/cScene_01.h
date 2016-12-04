#pragma once

#include "cScene.h"

class cScene_01 : public cScene
{
private:
	typedef struct tagMYVERTEX
	{
		D3DXVECTOR3 pos;
		DWORD		color;

		enum{FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE};
	}MYVERTEX, *LPMYVERTEX;

	//���� �����͸� ������ ���ؽ����� ������
	LPDIRECT3DVERTEXBUFFER9 vb;

public:
	cScene_01();
	~cScene_01();

	virtual void Init();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();

private:
};

