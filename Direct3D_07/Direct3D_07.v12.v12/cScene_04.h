#pragma once

#include "cScene.h"

class cScene_04 : public cScene
{
private:
	//Animation Ű������ ����ü
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

	//�����ϳ��� ���� ������ �����ϴ� ����ü
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//������ ��ġ
		DWORD		color;			//������ �÷�

		//���� ������ ������ ��Ÿ���� �÷��� ����� 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ ������ ��ġ���� | D3DFVF_DIFFUSE ������ �÷�����
	}MYVERTEX, *LPMYVERTEX;


	LPDIRECT3DVERTEXBUFFER9			vb;				//������ ������ ���� ����
	LPDIRECT3DINDEXBUFFER9			ib;				//���� �ε��������� ������ �ε��� ����

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

