#pragma once

#include "cScene.h"

class cScene_00 : public cScene
{
private:
	typedef struct tagMYVERTEX
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;

		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1};
	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9 m_vb;
	LPDIRECT3DINDEXBUFFER9 m_ib;

	//�ε��� texture (png, tga, bmp, jpg, dds) �ε� ���� (dds Direct3D ���� ����)
	LPDIRECT3DTEXTURE9 m_tex;

public:
	cScene_00();
	~cScene_00();

	virtual void Init();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();
};

