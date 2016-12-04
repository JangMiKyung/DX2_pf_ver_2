#pragma once

#include "cSingletonBase.h"

class cSpriteManager : public cSingletonBase<cSpriteManager>
{
private:
	LPD3DXSPRITE m_pSprite;

public:
	cSpriteManager();
	~cSpriteManager();

	HRESULT Init(LPDIRECT3DDEVICE9	pDevice);
	void Release();

	void BeginSpriteRender();		//스플라이트 랜더 시작시 호출
	void EndSpriteRender();			//스플라이트 랜더 종료시 호출

	void DrawTexture(
		LPDIRECT3DTEXTURE9 pTex,			//그릴 Texture
		LPRECT pSour,						//그릴 소스 영역
		float posX, float posY,				//그릴 화면 위치
		DWORD color = 0xffffffff,			//섞이는 색상
		D3DXVECTOR3* pCenter = NULL			//이미지의 중심 ( NULL 이면 좌상단 )
		);


	void DrawTexture(
		LPDIRECT3DTEXTURE9 pTex,			//그릴 Texture
		LPRECT pSour,						//그릴 소스 영역
		float posX, float posY,				//그릴 화면 위치
		float scaleX, float scaleY,			//그릴 스케일 크기
		float rotate,						//그릴 로테이션 값
		DWORD color = 0xffffffff,			//섞이는 색상
		D3DXVECTOR3* pCenter = NULL			//이미지의 중심 ( NULL 이면 좌상단 )
		);


	void DrawArea(LPDIRECT3DTEXTURE9 pTex,
		LPRECT pSour,						//그릴소스의 영역
		LPRECT pDest,						//그릴화면의 영역
		DWORD color = 0xffffffff			//섞이는 색상 
		);


	void DrawNinePatch(
		LPDIRECT3DTEXTURE9 pTex,
		LPRECT pSour,						//그릴 소스 영역
		LPRECT pDest,						//그릴 화면 영역
		LPRECT pCenter,						//나인패치 센터 영역
		DWORD color = 0xffffffff,			//섞이는 색상 
		bool bDrawCenter = true				//중앙 그리는지 안그리는지 여부
		);


	void DrawLoop(
		LPDIRECT3DTEXTURE9 pTex,
		float offsetX, float offsetY,		//SourOffset 
		LPRECT pSour,						//그릴 소스 영역
		LPRECT pDest,						//그릴 화면 영역
		DWORD color = 0xffffffff			//섞이는 색상 
		);
};

#define SPRITE_MGR cSpriteManager::GetInstance()