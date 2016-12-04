#include "stdafx.h"
#include "cDXFontManager.h"


cDXFontManager::cDXFontManager()
	: m_pDefaultFont(NULL)
{
}


cDXFontManager::~cDXFontManager()
{
}

HRESULT cDXFontManager::Init(LPDIRECT3DDEVICE9 pDevice)
{
	//��Ʈ ��ü �����
	if (FAILED(D3DXCreateFont(
		pDevice,			//����̽�
		30.f,				//��Ʈ�� ����
		0.f,				//��Ʈ�� ��(�� ���� 0�̸� �տ��� ������ ��Ʈ ���̿� ���)
		FW_BOLD,			//��Ʈ�� �β� ����(FW_BOLD ��Ʈ�� BOLD ��Ÿ�� ũ�⸦ ���´�)
		1,					//�Ӹ�(�׳� 1)
		FALSE,				//���ڸ� ����
		DEFAULT_CHARSET,	//ĳ���� set( �׳� DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//�ܰ� ó����� ( �׳� OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//����Ƽ ( �׳� DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// �۾�ü �йи��� ����
		"Bitstream Vera Sans Mono",			//���� ��Ʈ �̸� 
		&m_pDefaultFont			//������ ��Ʈ��ü ���� ���� ������
		)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void cDXFontManager::Release()
{
	SAFE_RELEASE(m_pDefaultFont);

	//�ʿ� �ִ� �༮�� �� �о������
	MAP_FONT_ITER iter;
	for (iter = m_FontStyleMap.begin(); iter != m_FontStyleMap.end(); ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
}

void cDXFontManager::addStyle(LPDIRECT3DDEVICE9 pDevice, std::string style, std::string fontName, float fontSize)
{
	MAP_FONT_ITER find = m_FontStyleMap.find(style);

	if (find != m_FontStyleMap.end())
		return;

	//���ο� ��Ʈ �߰�
	LPD3DXFONT newFont;
	D3DXCreateFont(
		pDevice,			//����̽�
		fontSize,				//��Ʈ�� ����
		0.f,				//��Ʈ�� ��(�� ���� 0�̸� �տ��� ������ ��Ʈ ���̿� ���)
		FW_BOLD,			//��Ʈ�� �β� ����(FW_BOLD ��Ʈ�� BOLD ��Ÿ�� ũ�⸦ ���´�)
		1,					//�Ӹ�(�׳� 1)
		FALSE,				//���ڸ� ����
		DEFAULT_CHARSET,	//ĳ���� set( �׳� DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//�ܰ� ó����� ( �׳� OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//����Ƽ ( �׳� DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// �۾�ü �йи��� ����
		fontName.c_str(),			//���� ��Ʈ �̸� 
		&newFont			//������ ��Ʈ��ü ���� ���� ������
		);

	m_FontStyleMap.insert(std::make_pair(style, newFont));

}

void cDXFontManager::PrintText(std::string str, int x, int y, DWORD color)
{
	//��� ����
	RECT rc = { x, y, 0, 0 };

	m_pDefaultFont->DrawText(
		NULL,
		str.c_str(),
		-1,
		&rc,
		DT_LEFT | DT_NOCLIP,
		color);
}

void cDXFontManager::PrintTextShadow(std::string str, int x, int y, DWORD color, DWORD shadow /*= 0xff000000*/)
{
	//������ ���� �׸���
	PrintText(str, x + 1, y + 1, shadow);

	PrintText(str, x, y, color);
}

void cDXFontManager::PrintTextOutline(std::string str, int x, int y, DWORD color, DWORD outLine /*= 0xff000000*/)
{
	//�ܰ� ��´�
	PrintText(str, x + 1, y, outLine);
	PrintText(str, x - 1, y, outLine);
	PrintText(str, x, y + 1, outLine);
	PrintText(str, x, y - 1, outLine);

	//���� Text ��´�
	PrintText(str, x, y, color);
}

void cDXFontManager::PrintStyleText(std::string style, std::string str, int x, int y, DWORD color)
{
	LPD3DXFONT pFont = NULL;

	MAP_FONT_ITER find = m_FontStyleMap.find(style);

	if (find == m_FontStyleMap.end()){
		pFont = m_pDefaultFont;
	}
	else{
		pFont = find->second;
	}

	RECT rc = { x, y, 0, 0 };

	pFont->DrawText(
		NULL,
		str.c_str(),
		-1,
		&rc,
		DT_LEFT | DT_NOCLIP,
		color);

}

void cDXFontManager::PrintStyleTextShadow(std::string style, std::string str, int x, int y, DWORD color, DWORD shadow /*= 0xff000000*/)
{
	PrintStyleText(style, str, x + 1, y + 1, shadow);

	PrintStyleText(style, str, x, y, color);
}

void cDXFontManager::PrintStyleTextOutline(std::string style, std::string str, int x, int y, DWORD color, DWORD outLine /*= 0xff000000*/)
{
	PrintStyleText(style, str, x + 1, y, outLine);
	PrintStyleText(style, str, x - 1, y, outLine);
	PrintStyleText(style, str, x, y + 1, outLine);
	PrintStyleText(style, str, x, y - 1, outLine);

	PrintStyleText(style, str, x, y, color);
}