#pragma once

#include "cxmesh.h"

#include "BoneHierachy.h"		//본하이어 라키
#include "cSkinnedAnimation.h"

class cTransform;
class cCamera;

class cXMesh_Skinned : public cXMesh
{
private:
	friend class BONE_HIERARCHY;							//본하이어 라키는 내친구....
	friend class cSkinnedAnimation;							//cSkinnedAnimation 는 내친구...
	typedef std::map< std::string, LPD3DXANIMATIONSET >			MAP_ANIMSET;
	typedef std::vector< LPD3DXANIMATIONSET >					VEC_ANIMSET;


public:
	static LPD3DXEFFECT				sSkinnedMeshEffect;				//StaticMesh 를 그리는 Effect


private:
	LPD3DXEFFECT				m_pSkinnedEffect;	//Skinned 메쉬 Effect;

	LPD3DXFRAME					m_pRootbone;		//Skinned 메쉬의 루트 본
	D3DXMATRIXA16				m_matCorrection;	//보정행렬

	DWORD						m_dwWorkingPaletteSize;		//행렬 작업 팔래트수
	D3DXMATRIX *				m_pmWorkingPalette;			//행렬 작업 팔래트 

	/////////////////////////////////////////////////////////////////////////////////

	LPD3DXANIMATIONCONTROLLER		m_pAnimController;			//Animation 컨트롤러
	UINT							m_AnimNum;					//Animation 갯수
	MAP_ANIMSET						m_mapAnimSet;				//Map 으로 되어있는 AnimationSet
	VEC_ANIMSET						m_vecAnimSet;				//Vector 로 되어있는 AnimationSet

	LPD3DXANIMATIONSET				m_pNowPlayAnimationSet;		//현제 플레이되고있는 AnimationSet
	D3DXTRACK_DESC					m_Track_Desc_0;				//0번 Track_Desc ( 0 번 Track 의 정보 )


	bool							m_bPlay;					//지금 플레이 중이니?
	bool							m_bLoop;					//루프 플레이니?
	LPD3DXANIMATIONSET				m_pPrevPlayAnimationSet;	//OneShot 플레이시 한번 Animation 플레이되고 다시 되돌아갈 Animaiton

	float							m_fCrossFadeTime;			//크로스페이드 타임
	float							m_fLeftCrossFadeTime;		//남은크로스페이드 타임
	float							m_fOutCrossFadeTime;		//OneShot Animation 끝나고 되돌아갈때 CrossFade 타임
	double							m_AnimationPlayFactor;		//Animation 진행 Factor ( 0 이면 처음 1 일수록 마지막 )

public:
	cXMesh_Skinned();
	~cXMesh_Skinned();

	virtual HRESULT Init(std::string filePath, const D3DXMATRIXA16* matCorrection = NULL);
	virtual void Release();

	void Update(float timeDelta);

	virtual void Render(const cTransform* pTrans);

	void RenderBoneName(cCamera* pCam, cTransform* transform = NULL);

	void Play(std::string animName, float crossFadeTime = 0.0);
	void Play(int animIndex, float crossFadeTime = 0.0);

	void PlayOneShot(std::string animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShotAfterHold(std::string animName, float crossFadeTime = 0.0);

	void Stop(){
		this->m_bPlay = false;
	}

	void SetPlaySpeed(float speed);

	//Animation 정보를 출력한다.
	void ShowAnimationName(int x, int y);

	//해당 인덱스의 Animation Set 을 얻는다.
	LPD3DXANIMATIONSET GetAnimSet(int index)
	{
		return m_vecAnimSet[index];
	}

	//본이름으로 본찾는다.
	BONE* GetFineBONE(std::string boneName);

private:

	//본 행렬을 셋팅한다.
	void InitBoneMatrixPointer(BONE* pBone);

	//본행렬 을 업데이트한다.
	void UpdateMatrices(BONE* pBone, D3DXMATRIXA16* pParentMatrix);

	//업데이트 된 본의 행렬되로 메쉬를 변형한다.
	void UpdateSkinnedMesh(BONE* pBone);

	//본을 그린다.
	void RenderBone(BONE* pBone);

	//본이름을 출력한다.
	void RenderChildBoneName(cCamera* pCam, BONE* pBone, D3DXVECTOR3* parentPos);

	//Animation 을 바꾼다.
	void SetAnimation(LPD3DXANIMATIONSET animSet);

public:
	static void SetCamera(const cCamera* Camera);

};

