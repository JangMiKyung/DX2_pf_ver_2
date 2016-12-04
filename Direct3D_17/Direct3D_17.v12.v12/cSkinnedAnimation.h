#pragma once

#include "cXMesh_Skinned.h"
class cCamera;

class cSkinnedAnimation
{
private:
	typedef std::map<BONE*, cTransform*>						MAP_BONETRANSFORM;		//매핑되는 폰Transform 맵
	typedef std::map< std::string, LPD3DXANIMATIONSET >			MAP_ANIMSET;
	typedef std::vector< LPD3DXANIMATIONSET >					VEC_ANIMSET;


private:
	cXMesh_Skinned*		m_pSkinnedMesh;
	MAP_BONETRANSFORM	m_mapApplyBoneTransform;
	MAP_BONETRANSFORM	m_mapBoneTransform;

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


	float							m_fAnimDelta;

public:
	HRESULT		Init(cXMesh_Skinned* pSkinnedMesh);
	void		Release();
	void		Update(float timeDelta);
	void		Render(cTransform* pTransform);
	void		RenderBoneName(cCamera* pCam, cTransform* pTransform);


	void Play(std::string animName, float crossFadeTime = 0.0);
	void Play(int animIndex, float crossFadeTime = 0.0);
	void Play(LPD3DXANIMATIONSET animSet, float crossFadeTime = 0.0);
	void PlayOneShot(std::string animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShotAfterHold(std::string animName, float crossFadeTime = 0.0);
	void Stop(){
		this->m_bPlay = false;
	}
	void SetPlaySpeed(float speed);


	void AddBoneTransform(std::string boneName, cTransform* pTransform);
	void AddApplyTransform(std::string boneName, cTransform* pTransform);
	void RemoveBoneTransform(std::string boneName);
	void RemoveApplyTransform(std::string boneName);


public:
	cSkinnedAnimation();
	~cSkinnedAnimation();


private:
	//Animation 을 바꾼다.
	void SetAnimation(LPD3DXANIMATIONSET animSet);
};

