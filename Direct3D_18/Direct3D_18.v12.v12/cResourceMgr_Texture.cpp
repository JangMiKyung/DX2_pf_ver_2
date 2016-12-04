#include "stdafx.h"
#include "cResourceMgr_Texture.h"


cResourceMgr_Texture::cResourceMgr_Texture()
{
}


cResourceMgr_Texture::~cResourceMgr_Texture()
{
}

//�ε� ��ĸ� ������ 
LPDIRECT3DTEXTURE9 cResourceMgr_Texture::LoadResource(std::string filePath, void* pParam /*= NULL*/)
{
	//�ؽ��� �ε�
	LPDIRECT3DTEXTURE9	pNewTex = NULL;
	if (FAILED(D3DXCreateTextureFromFile(Device, filePath.c_str(), &pNewTex)))
		return NULL;

	return pNewTex;
}

//���� ��ĸ� ������
void cResourceMgr_Texture::ReleaseResource(LPDIRECT3DTEXTURE9 data)
{
	SAFE_RELEASE(data);
}