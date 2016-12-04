#pragma once
#include "cScene.h"
class cScene_01 : public cScene
{
private:
	//정점하나에 대한 정보를 정의하는 구조체
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//정점의 위치
		D3DXVECTOR2 uv;				//정점 UV 좌표	

		//현제 정점의 정보를 나타내는 플래그 상수값 
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };	//D3DFVF_XYZ 정점의 위치정보 | D3DFVF_TEX1 정점의 TEXTURE UV 좌표는 1 개다...
	}MYVERTEX, *LPMYVERTEX;


	LPDIRECT3DVERTEXBUFFER9			m_vb;
	LPDIRECT3DINDEXBUFFER9			m_ib;

	//로딩된 Texture  ( png, tga, bmp, jpg, dds ) 로딩 가능 Texture 포맷 (dds Direct3D 전용 Texture 포맷)
	LPDIRECT3DTEXTURE9				m_pTex;


	cCamera							m_mainCamera;
	cTransform						m_quadTrans;


	LPD3DXEFFECT					pEffect;			//로딩된 셰이더

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

