#pragma once

#include "cScene.h"

class cScene_04 : public cScene
{
private:
	//Animation 키프레임 구조체
	typedef struct tagKEY_FRAME{

		int				frame;
		cTransform		trans;

		tagKEY_FRAME(){
			frame = 0;
		}

		tagKEY_FRAME(int frame, const cTransform& other){
			this->frame = frame;
			this->trans = other;
		}

	}KEY_FRAME, *LPKEY_FRAME;

private:
	typedef std::map<std::string, cTransform*> MAP_TRANS;
	typedef MAP_TRANS::iterator				  MAP_TRANS_ITER;

	typedef std::vector<LPKEY_FRAME>				KEY_FRAMES;
	typedef std::map<std::string, KEY_FRAMES>		MAP_BONE_KEYFRAMES;

	//정점하나에 대한 정보를 정의하는 구조체
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//정점의 위치
		DWORD		color;			//정점의 컬러

		//현제 정점의 정보를 나타내는 플래그 상수값 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ 정점의 위치정보 | D3DFVF_DIFFUSE 정점의 컬러정보
	}MYVERTEX, *LPMYVERTEX;


	LPDIRECT3DVERTEXBUFFER9			vb;				//정점을 보관할 정점 버퍼
	LPDIRECT3DINDEXBUFFER9			ib;				//정점 인덱스정보를 보관할 인덱스 버퍼

	MAP_TRANS						BoneMap;
	MAP_TRANS						MeshMap;
	cTransform						Trans;


	cCamera							mainCamera;

	float							nowFrame;
	MAP_BONE_KEYFRAMES				animationFrames;

public:
	cScene_04();
	~cScene_04();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:

	void CreateCube();

	void RenderCube();
};

