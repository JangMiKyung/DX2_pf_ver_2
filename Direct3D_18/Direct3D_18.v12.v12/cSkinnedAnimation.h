#pragma once

#include "cXMesh_Skinned.h"
#include <vector>
class cCamera;

class cSkinnedAnimation
{
private:
	typedef std::map<BONE*, cTransform*>						MAP_BONETRANSFORM;		//���εǴ� ��Transform ��
	typedef std::map< std::string, LPD3DXANIMATIONSET >			MAP_ANIMSET;
	typedef std::map< std::string, LPD3DXANIMATIONSET >::iterator				MAPITER_ANIMSET;
	typedef std::vector< LPD3DXANIMATIONSET >					VEC_ANIMSET;
	typedef std::vector< LPD3DXANIMATIONSET >::iterator					VECITER_ANIMSET;
private:
	cXMesh_Skinned*		m_pSkinnedMesh;
	MAP_BONETRANSFORM	m_mapApplyBoneTransform;
	MAP_BONETRANSFORM	m_mapBoneTransform;

	/////////////////////////////////////////////////////////////////////////////////

	LPD3DXANIMATIONCONTROLLER		m_pAnimController;			//Animation ��Ʈ�ѷ�
	UINT							m_AnimNum;					//Animation ����
	MAP_ANIMSET						m_mapAnimSet;				//Map ���� �Ǿ��ִ� AnimationSet
	VEC_ANIMSET						m_vecAnimSet;				//Vector �� �Ǿ��ִ� AnimationSet
	VEC_ANIMSET                      m_vecComAnimSet;
	VECITER_ANIMSET                 m_viAnimeSet;

	LPD3DXANIMATIONSET				m_pNowPlayAnimationSet;		//���� �÷��̵ǰ��ִ� AnimationSet
	D3DXTRACK_DESC					m_Track_Desc_0;				//0�� Track_Desc ( 0 �� Track �� ���� )

	bool							m_bPlay;					//���� �÷��� ���̴�?
	bool							m_bLoop;					//���� �÷��̴�?
	LPD3DXANIMATIONSET				m_pPrevPlayAnimationSet;	//OneShot �÷��̽� �ѹ� Animation �÷��̵ǰ� �ٽ� �ǵ��ư� Animaiton

	float							m_fCrossFadeTime;			//ũ�ν����̵� Ÿ��
	float							m_fLeftCrossFadeTime;		//����ũ�ν����̵� Ÿ��
	float							m_fOutCrossFadeTime;		//OneShot Animation ������ �ǵ��ư��� CrossFade Ÿ��

	double							m_AnimationPlayFactor;		//Animation ���� Factor ( 0 �̸� ó�� 1 �ϼ��� ������ )


	float							m_fAnimDelta;
	bool                          isAniFinish;
	bool isComboAni;



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
	void PlayOneShot(LPD3DXANIMATIONSET animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShotAfterHold(std::string animName, float crossFadeTime = 0.0);
	void PlayAniCombo();
	void Stop() {
		this->m_bPlay = false;
	}
	void SetPlaySpeed(float speed);
	void pushAniCombo(string Anime);

	void AddBoneTransform(std::string boneName, cTransform* pTransform);
	void AddApplyTransform(std::string boneName, cTransform* pTransform);
	void RemoveBoneTransform(std::string boneName);
	void RemoveApplyTransform(std::string boneName);
	bool isPlay() { return m_bPlay;  }

	void setComStart() { isComboAni = true; }

	void setPreAni(string str) { m_pPrevPlayAnimationSet = (*m_mapAnimSet.find(str)).second;  }

public:
	cSkinnedAnimation();
	~cSkinnedAnimation();


private:
	//Animation �� �ٲ۴�.
	void SetAnimation(LPD3DXANIMATIONSET animSet);
};

