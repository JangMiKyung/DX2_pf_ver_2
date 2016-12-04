#pragma once

class cImage
{
private:
	HDC			m_BitDC;
	HBITMAP		m_NewBit;
	HBITMAP		m_OldBit;
	DWORD		m_dwWidth;
	DWORD		m_dwHeight;
	COLORREF	m_TransColorKey;		//���÷�Ű( -1 ) �̸� ���� ����

	HDC			m_BlendDC;		//����� DC
	HBITMAP		m_NewBlendBit;
	HBITMAP		m_OldBlendBit;

	//���� ����ü
	BLENDFUNCTION	m_BlendFunc;



public:
	cImage( void );
	~cImage( void );

	//��Ʈ�� �����Լ�
	HRESULT	Init( DWORD dwWidth, DWORD dwHeight );
	HRESULT Init( const char* szFileName, DWORD dwWidth, DWORD dwHeight, COLORREF transColorKey = -1 );

	//��Ʈ�� ���� �Լ�
	void Release( void );

	//��Ʈ�� �׸���
	void Draw( HDC hdc, int x, int y );
	void Draw( HDC hdc, int x, int y, int sourX, int sourY, int sourW, int sourH );
	void Draw( HDC hdc, int x, int y, RECT& rcSour );

	void DrawAlpha( HDC hdc, int x, int y, BYTE alpha );
	void DrawAlpha( HDC hdc, int x, int y, int sourX, int sourY, int sourW, int sourH, BYTE alpha );
	void DrawAlpha( HDC hdc, int x, int y, RECT& rcSour, BYTE alpha );

	//�ݺ���ο�
	void LoopDraw( HDC hdc, int offsetX, int offsetY, RECT& rcDest );
	void LoopDrawAlpha( HDC hdc, int offsetX, int offsetY, RECT& rcDest, BYTE alpha );

	//��Ʈ�� DC �� ��´�
	HDC GetBitDC( void ) { return m_BitDC; }
};



