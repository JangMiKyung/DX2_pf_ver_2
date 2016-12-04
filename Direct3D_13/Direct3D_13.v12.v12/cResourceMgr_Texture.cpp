#include "stdafx.h"
#include "cResourceMgr_Texture.h"


cResourceMgr_Texture::cResourceMgr_Texture()
{
}


cResourceMgr_Texture::~cResourceMgr_Texture()
{
}

//로드 방식만 재정의 
LPDIRECT3DTEXTURE9 cResourceMgr_Texture::LoadResource(std::string filePath, void* pParam /*= NULL*/)
{
	//텍스쳐 로딩
	LPDIRECT3DTEXTURE9	pNewTex = NULL;
	if (FAILED(D3DXCreateTextureFromFile(Device, filePath.c_str(), &pNewTex)))
		return NULL;

	return pNewTex;
}

//해재 방식만 재정의
void cResourceMgr_Texture::ReleaseResource(LPDIRECT3DTEXTURE9 data)
{
	SAFE_RELEASE(data);
}