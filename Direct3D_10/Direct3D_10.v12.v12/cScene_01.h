#pragma once
#include "cScene.h"
class cScene_01 : public cScene
{
private:
	//�����ϳ��� ���� ������ �����ϴ� ����ü
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//������ ��ġ
		D3DXVECTOR2 uv;				//���� UV ��ǥ	

		//���� ������ ������ ��Ÿ���� �÷��� ����� 
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };	//D3DFVF_XYZ ������ ��ġ���� | D3DFVF_TEX1 ������ TEXTURE UV ��ǥ�� 1 ����...
	}MYVERTEX, *LPMYVERTEX;


	LPDIRECT3DVERTEXBUFFER9			m_vb;
	LPDIRECT3DINDEXBUFFER9			m_ib;

	//�ε��� Texture  ( png, tga, bmp, jpg, dds ) �ε� ���� Texture ���� (dds Direct3D ���� Texture ����)
	LPDIRECT3DTEXTURE9				m_pTex;


	cCamera							m_mainCamera;
	cTransform						m_quadTrans;


	LPD3DXEFFECT					pEffect;			//�ε��� ���̴�

public:
	cScene_01();
	~cScene_01();

	virtual HRESULT Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:

	void CreateQuad();
};

