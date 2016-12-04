#pragma once

#include "cSingletonBase.h"
#include <string>
#include <map>

class cDXFontManager : public cSingletonBase<cDXFontManager>
{
private:
	typedef std::map<std::string, LPD3DXFONT>	MAP_FONT;
	typedef MAP_FONT::iterator					MAP_FONT_ITER;

private:
	LPD3DXFONT	m_pDefaultFont;//디폴트 폰트
	MAP_FONT	m_FontStyleMap;//폰트 스타일맵

public:
	cDXFontManager();
	~cDXFontManager();

	HRESULT Init(LPDIRECT3DDEVICE9 pDevice);
	void Release();

	void addStyle(LPDIRECT3DDEVICE9 pDevice, std::string style, std::string fontName, float fontSize);

	void PrintText(std::string str, int x, int y, DWORD color);
	void PrintTextShadow(std::string str, int x, int y, DWORD color, DWORD shadow = 0xff000000);
	void PrintTextOutline(std::string str, int x, int y, DWORD color, DWORD outLine = 0xff000000);

	void PrintStyleText(std::string style, std::string str, int x, int y, DWORD color);
	void PrintStyleTextShadow(std::string style, std::string str, int x, int y, DWORD color, DWORD shadow = 0xff000000);
	void PrintStyleTextOutline(std::string style, std::string str, int x, int y, DWORD color, DWORD outLine = 0xff000000);
};

#define DXFONT_MGR cDXFontManager::GetInstance()
