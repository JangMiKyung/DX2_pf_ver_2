#pragma once
#include "cResourceMgr.h"
class cResourceMgr_Texture : public cResourceMgr<LPDIRECT3DTEXTURE9, cResourceMgr_Texture>
{
public:
	cResourceMgr_Texture();
	~cResourceMgr_Texture();

	//로드 방식만 재정의 
	LPDIRECT3DTEXTURE9 LoadResource(std::string filePath, void* pParam = NULL) override;

	//해재 방식만 재정의
	void ReleaseResource(LPDIRECT3DTEXTURE9 data) override;
};

#define RESOURCE_TEXTURE cResourceMgr_Texture::GetInstance()