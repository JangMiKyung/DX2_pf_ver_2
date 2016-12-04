#pragma once
#include "cResourceMgr.h"
class cResourceMgr_Texture : public cResourceMgr<LPDIRECT3DTEXTURE9, cResourceMgr_Texture>
{
public:
	cResourceMgr_Texture();
	~cResourceMgr_Texture();

	//�ε� ��ĸ� ������ 
	LPDIRECT3DTEXTURE9 LoadResource(std::string filePath, void* pParam = NULL) override;

	//���� ��ĸ� ������
	void ReleaseResource(LPDIRECT3DTEXTURE9 data) override;
};

#define RESOURCE_TEXTURE cResourceMgr_Texture::GetInstance()