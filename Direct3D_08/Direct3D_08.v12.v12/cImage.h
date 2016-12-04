#pragma once

class cImage
{
private:
	HDC			m_BitDC;
	HBITMAP		m_NewBit;
	HBITMAP		m_OldBit;
	DWORD		m_dwWidth;
	DWORD		m_dwHeight;
	COLORREF	m_TransColorKey;		//뺄컬러키( -1 ) 이면 없는 것임

	HDC			m_BlendDC;		//블렌드용 DC
	HBITMAP		m_NewBlendBit;
	HBITMAP		m_OldBlendBit;

	//블렌드 구조체
	BLENDFUNCTION	m_BlendFunc;



public:
	cImage( void );
	~cImage( void );

	//비트맵 셋팅함수
	HRESULT	Init( DWORD dwWidth, DWORD dwHeight );
	HRESULT Init( const char* szFileName, DWORD dwWidth, DWORD dwHeight, COLORREF transColorKey = -1 );

	//비트맵 해제 함수
	void Release( void );

	//비트맵 그리기
	void Draw( HDC hdc, int x, int y );
	void Draw( HDC hdc, int x, int y, int sourX, int sourY, int sourW, int sourH );
	void Draw( HDC hdc, int x, int y, RECT& rcSour );

	void DrawAlpha( HDC hdc, int x, int y, BYTE alpha );
	void DrawAlpha( HDC hdc, int x, int y, int sourX, int sourY, int sourW, int sourH, BYTE alpha );
	void DrawAlpha( HDC hdc, int x, int y, RECT& rcSour, BYTE alpha );

	//반복드로우
	void LoopDraw( HDC hdc, int offsetX, int offsetY, RECT& rcDest );
	void LoopDrawAlpha( HDC hdc, int offsetX, int offsetY, RECT& rcDest, BYTE alpha );

	//비트맵 DC 를 얻는다
	HDC GetBitDC( void ) { return m_BitDC; }
};



