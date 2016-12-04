#include "stdafx.h"
#include "cSpriteManager.h"


cSpriteManager::cSpriteManager()
{
}


cSpriteManager::~cSpriteManager()
{
}

HRESULT cSpriteManager::Init(LPDIRECT3DDEVICE9	pDevice)
{
	//스플라이트 개체 생성
	if (FAILED(D3DXCreateSprite(pDevice, &m_pSprite)))
		return E_FAIL;

	return S_OK;
}

void cSpriteManager::Release()
{
	//스플라이트 개체 해제
	SAFE_RELEASE(m_pSprite);
}

void cSpriteManager::BeginSpriteRender()		//스플라이트 랜더 시작시 호출
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void cSpriteManager::EndSpriteRender()			//스플라이트 랜더 종료시 호출
{
	m_pSprite->End();
}

void cSpriteManager::DrawTexture(
	LPDIRECT3DTEXTURE9 pTex,			//그릴 Texture
	LPRECT pSour,						//그릴 소스 영역
	float posX, float posY,				//그릴 화면 위치
	DWORD color /*= 0xffffffff*/,		//섞이는 색상
	D3DXVECTOR3* pCenter /*= NULL*/		//이미지의 중심 ( NULL 이면 좌상단 )
	)
{
	//화면 절대 좌표로 그리니 SpriteCanvas 의 Transform 을 초기화
	D3DXMATRIXA16 matFinal;
	D3DXMatrixIdentity(&matFinal);
	m_pSprite->SetTransform(&matFinal);

	//그릴화면위치 ( 주의사항 Vector2 가아니라 Vector3 이다, Z 는 일반적으로 무시되는데 깊이를 주고싶으면 사용해라... )
	D3DXVECTOR3 pos(posX, posY, 0.0f);

	//스플라이트 개페를 통해 Texture 를 화면에 그린다...
	m_pSprite->Draw(
		pTex,		//그릴 Texture
		pSour,		//그릴 이미지의 소스 영역
		(pCenter == NULL) ? &D3DXVECTOR3(0, 0, 0) : pCenter,		//그리는 이미의 중심위치
		&pos,		//그릴화면 위치 
		color		//Hit 컬러
		);
}


void cSpriteManager::DrawTexture(
	LPDIRECT3DTEXTURE9 pTex,			//그릴 Texture
	LPRECT pSour,						//그릴 소스 영역
	float posX, float posY,				//그릴 화면 위치
	float scaleX, float scaleY,			//그릴 스케일 크기
	float rotate,						//그릴 로테이션 값
	DWORD color /*= 0xffffffff*/,		//섞이는 색상
	D3DXVECTOR3* pCenter /*= NULL*/		//이미지의 중심 ( NULL 이면 좌상단 )
	)
{
	//이미지의 기준점
	D3DXVECTOR3 pivot =
		(pCenter == NULL) ? D3DXVECTOR3(0, 0, 0) : *pCenter;

	//스케일 행렬
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale,
		scaleX, scaleY, 1.0f);

	//이동 행렬
	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans,
		posX, posY, 0.0f);


	//회전 행렬
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationZ(&matRot,
		rotate);

	//최종 행렬
	D3DXMATRIXA16 matFinal;
	matFinal = matScale * matRot * matTrans;

	//스케일 적용
	m_pSprite->SetTransform(&matFinal);


	//이미지 그려라
	m_pSprite->Draw(
		pTex,
		pSour,
		&pivot,
		&D3DXVECTOR3(0, 0, 0),	//그릴 위치가 0 이네....
		color);
}


void cSpriteManager::DrawArea(LPDIRECT3DTEXTURE9 pTex,
	LPRECT pSour,						//그릴소스의 영역
	LPRECT pDest,						//그릴화면의 영역
	DWORD color /*= 0xffffffff*/		//섞이는 색상 
	)
{
	float destWidth = pDest->right - pDest->left;
	float destHeight = pDest->bottom - pDest->top;

	float sourWidth = pSour->right - pSour->left;
	float sourHeight = pSour->bottom - pSour->top;

	//영역 비율에 의한 스케일 값
	float scaleX = destWidth / sourWidth;
	float scaleY = destHeight / sourHeight;


	//스케일 행렬
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale,
		scaleX, scaleY, 1.0f);


	//이동행렬
	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(
		&matTrans,
		pDest->left, pDest->top, 0.0f);

	D3DXMATRIXA16 matFinal = matScale * matTrans;
	m_pSprite->SetTransform(&matFinal);


	//이미지 그려라
	m_pSprite->Draw(
		pTex,
		pSour,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),	//그릴 위치가 0 이네....
		color);

}


void cSpriteManager::DrawNinePatch(
	LPDIRECT3DTEXTURE9 pTex,
	LPRECT pSour,						//그릴 소스 영역
	LPRECT pDest,						//그릴 화면 영역
	LPRECT pCenter,						//나인패치 센터 영역
	DWORD color /*= 0xffffffff*/,		//섞이는 색상 
	bool bDrawCenter /*= true*/			//중앙 그리는지 안그리는지 여부
	)
{
	float fCenterW = pCenter->right - pCenter->left;
	float fCenterH = pCenter->bottom - pCenter->top;

	//소스영역
	RECT rcSrcLeftTop		= { 0,				0,				pCenter->left,	pCenter->top };
	RECT rcSrcTop			= { pCenter->left,	0,				pCenter->right, pCenter->top };
	RECT rcSrcRightTop		= { pCenter->right, 0,				pSour->right,	pCenter->top };
	RECT rcSrcLeftCenter	= { 0,				pCenter->top,	pCenter->left,	pCenter->bottom };
	RECT rcSrcCenter		= { pCenter->left,	pCenter->top,	pCenter->right, pCenter->bottom };
	RECT rcSrcRightCenter	= { pCenter->right, pCenter->top,	pSour->right,	pCenter->bottom };
	RECT rcSrcLeftBottom	= { 0,				pCenter->bottom,pCenter->left,	pSour->bottom };
	RECT rcSrcBottom		= { pCenter->left,	pCenter->bottom,pCenter->right, pSour->bottom };
	RECT rcSrcRightBottom	= { pCenter->right, pCenter->bottom,pSour->right,	pSour->bottom };

	//그리기 영역
	//전체
	float left = pDest->left;
	float top = pDest->top;
	float right = pDest->right;
	float bottom = pDest->bottom;

	float centerLeft	= pDest->left + (pCenter->left - pSour->left);
	float centerRight	= pDest->right - (pSour->right - pCenter->right);
	float centerTop		= pDest->top + (pCenter->top - pSour->top);
	float centerBottom	= pDest->bottom - (pSour->bottom - pCenter->bottom);


	RECT rcDstLeftTop		= { left,			top,			centerLeft,		centerTop };
	RECT rcDstTop			= { centerLeft,		top,			centerRight,	centerTop };
	RECT rcDstRightTop		= { centerRight,	top,			right,			centerTop };
	RECT rcDstLeftCenter	= { left,			centerTop,		centerLeft,		centerBottom };
	RECT rcDstCenter		= { centerLeft,		centerTop,		centerRight,	centerBottom };
	RECT rcDstRightCenter	= { centerRight,	centerTop,		right,			centerBottom };
	RECT rcDstLeftBottom	= { left,			centerBottom,	centerLeft,		bottom };
	RECT rcDstBottom		= { centerLeft,		centerBottom,	centerRight,	bottom };
	RECT rcDstRightBottom	= { centerRight,	centerBottom,	right,			bottom };

	DrawArea(pTex, &rcSrcLeftTop, &rcDstLeftTop, color);
	DrawArea(pTex, &rcSrcTop, &rcDstTop, color);
	DrawArea(pTex, &rcSrcRightTop, &rcDstRightTop, color);
	DrawArea(pTex, &rcSrcLeftCenter, &rcDstLeftCenter, color);

	//가운데는 true 일때만 그린다.
	if (bDrawCenter)
		DrawArea(pTex, &rcSrcCenter, &rcDstCenter, color);

	DrawArea(pTex, &rcSrcRightCenter, &rcDstRightCenter, color);
	DrawArea(pTex, &rcSrcLeftBottom, &rcDstLeftBottom, color);
	DrawArea(pTex, &rcSrcBottom, &rcDstBottom, color);
	DrawArea(pTex, &rcSrcRightBottom, &rcDstRightBottom, color);
}


void cSpriteManager::DrawLoop(
	LPDIRECT3DTEXTURE9 pTex,
	float fOffsetX, float fOffsetY,		//SourOffset 
	LPRECT pSour,						//그릴 소스 영역
	LPRECT pDest,						//그릴 화면 영역
	DWORD color /*= 0xffffffff*/		//섞이는 색상 
	)
{
	//소스의 가로크기
	int sourW = pSour->right - pSour->left;
	int sourH = pSour->bottom - pSour->top;

	//소스 Offset 구하자
	int offSetX = static_cast<int>(fOffsetX) % sourW;
	int offSetY = static_cast<int>(fOffsetY) % sourH;

	//음수 오프셋 위치
	if (offSetX < 0)
		offSetX = sourW + offSetX;
	if (offSetY < 0)
		offSetY = sourH + offSetY;

	//데스트의 가로크기 ( 총그릴 량이 된다 )
	int destW = pDest->right - pDest->left;
	int destH = pDest->bottom - pDest->top;

	//현재 한번그려진량 ( 루프 한번 돌때마다 그려진 량 )
	int drawX = 0;
	int drawY = 0;

	//계산된 소스 영역
	RECT rcDrawSour = { 0, 0, 0, 0 };
	RECT rcDrawDest = { 0, 0, 0, 0 };


	//가로 포문
	for (int x = 0; x < destW; x += drawX)
	{
		//가로 소스 영역을 구한다.
		rcDrawSour.left = (offSetX + x) % sourW + pSour->left;
		rcDrawSour.right = pSour->right;

		//드로우 되는 가로 량은 
		drawX = rcDrawSour.right - rcDrawSour.left;

		//가로로 총 그려지는 량이 넘어갔다면 
		if (x + drawX > destW){
			//넘어간 량만큼 오른쪽을 왼쪽 땡긴다.
			int overW = (x + drawX) - destW;
			rcDrawSour.right -= overW;

			//드로우 되는 가로량 갱신
			drawX = rcDrawSour.right - rcDrawSour.left;
		}



		//가로 데스트 영역을 구한다.
		rcDrawDest.left = pDest->left + x;
		rcDrawDest.right = rcDrawDest.left + drawX;




		for (int y = 0; y < destH; y += drawY)
		{
			//세로 소스 영역을 구한다.
			rcDrawSour.top = (offSetY + y) % sourH + pSour->top;
			rcDrawSour.bottom = pSour->bottom;

			//드로우 되는 세로 량은 
			drawY = rcDrawSour.bottom - rcDrawSour.top;

			//세로로 총 그려지는 량이 넘어갔다면 
			if (y + drawY > destH){

				//넘어간 량만큼 아래를 위쪽으로 땡긴다.
				int overH = (y + drawY) - destH;
				rcDrawSour.bottom -= overH;

				//드로우 되는 세로 량 갱신
				drawY = rcDrawSour.bottom - rcDrawSour.top;
			}


			//세로 데스트 영역을 구한다.
			rcDrawDest.top = pDest->top + y;
			rcDrawDest.bottom = rcDrawDest.top + drawY;


			//소스영역과 데스트영역을 이용하여 그린다.
			DrawArea(pTex, &rcDrawSour, &rcDrawDest, color);
		}
	}
}