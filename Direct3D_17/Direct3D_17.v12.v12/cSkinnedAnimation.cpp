#include "stdafx.h"
#include "cSkinnedAnimation.h"
#include "cTransform.h"
#include "cCamera.h"

cSkinnedAnimation::cSkinnedAnimation()
{
	m_mapApplyBoneTransform.clear();
	m_mapBoneTransform.clear();
}


cSkinnedAnimation::~cSkinnedAnimation()
{
}

HRESULT	cSkinnedAnimation::Init(cXMesh_Skinned* pSkinnedMesh)
{
	if (pSkinnedMesh == NULL) return E_FAIL;

	m_pSkinnedMesh = pSkinnedMesh;

	//SKinned Mesh 에 Animation 를 복사한다.
	m_pSkinnedMesh->m_pAnimController->CloneAnimationController(
		m_pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		m_pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		m_pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		m_pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);

	//Animation 갯수를 얻는다.
	m_AnimNum = m_pAnimController->GetNumAnimationSets();

	for (UINT i = 0; i < m_AnimNum; i++)
	{
		//에니메이션 셋 정보
		LPD3DXANIMATIONSET animSet;

		//i 번째 Animation 셋 정보를 얻는다.
		m_pAnimController->GetAnimationSet(i, &animSet);

		//AnimationSet Vector 에 저장
		this->m_vecAnimSet.push_back(animSet);

		//AnimationSet Map 에 저장
		this->m_mapAnimSet.insert(std::make_pair(
			animSet->GetName(),
			animSet));
	}

	//일단 첫번째 Animation 으로 시작
	this->Play(0);
}

void cSkinnedAnimation::Release()
{
	SAFE_RELEASE(m_pAnimController);
}

void cSkinnedAnimation::Update(float timeDelta)
{
	//0 번 Track 정보를 얻는다
	m_pAnimController->GetTrackDesc(0, &m_Track_Desc_0);

	// m_pNowPlayAnimationSet->GetPeriod() Animation 의 총시간을 얻는다.
	//현재 Animation 진행 Factor 를 갱신
	m_AnimationPlayFactor = m_Track_Desc_0.Position / m_pNowPlayAnimationSet->GetPeriod();


	//마지막에 도달했다면...
	if (m_AnimationPlayFactor >= 0.95f)
	{
		if (this->m_bLoop == false){

			//돌아갈 Animation 이 있다면..
			if (this->m_pPrevPlayAnimationSet != NULL)
			{
				m_fCrossFadeTime = m_fOutCrossFadeTime;
				m_fLeftCrossFadeTime = m_fOutCrossFadeTime;
				m_bLoop = true;
				this->SetAnimation(m_pPrevPlayAnimationSet);
				this->m_pPrevPlayAnimationSet = NULL;
			}

			//돌아갈 Animation 이 없다면.
			else
			{
				this->Stop();
			}
		}
	}

	m_AnimationPlayFactor = m_AnimationPlayFactor - (int)m_AnimationPlayFactor;	//정수부분 안생기게....


	if (m_bPlay)
	{
		//애니메이션을 진행시킴
		//m_pAnimController->AdvanceTime( timeDelta, NULL );
		m_fAnimDelta = timeDelta;
	}
	else
	{
		m_fAnimDelta = 0.0f;
	}


	//크로스 페이드가 진행중이라면..
	if (m_fLeftCrossFadeTime > 0.0f)
	{
		//남은 크로스페이드 시간 뺀다
		this->m_fLeftCrossFadeTime -= timeDelta;

		//크로스페이드 가끝났다.
		if (m_fLeftCrossFadeTime <= 0.0f)
		{
			m_pAnimController->SetTrackWeight(0, 1);
			m_pAnimController->SetTrackEnable(1, false);
		}

		else
		{
			float w1 = (m_fLeftCrossFadeTime / m_fCrossFadeTime);		//1번 Track 가중치
			float w0 = 1.0f - w1;										//0번 Track 가중치

			//가중치 셋팅
			m_pAnimController->SetTrackWeight(0, w0);
			m_pAnimController->SetTrackWeight(1, w1);
		}
	}

}


void cSkinnedAnimation::Render(cTransform* pTransform)
{
	//현재 자신의 Animation 정보로 셋팅
	m_pAnimController->AdvanceTime(m_fAnimDelta, NULL);
	m_fAnimDelta = 0.0f;		//애니메이션을 진행시켰기 때문에 delta 량은 0 으로....

	//각 본에 행렬 대입
	MAP_BONETRANSFORM::iterator iterBone;
	for (iterBone = m_mapBoneTransform.begin(); iterBone != m_mapBoneTransform.end(); ++iterBone)
		iterBone->first->pBoneTransform = iterBone->second;

	for (iterBone = m_mapApplyBoneTransform.begin(); iterBone != m_mapApplyBoneTransform.end(); ++iterBone)
		iterBone->first->pApplyTransform = iterBone->second;

	m_pSkinnedMesh->Render(pTransform);
}

void cSkinnedAnimation::RenderBoneName(cCamera* pCam, cTransform* pTransform)
{
	//현재 자신의 Animation 정보로 셋팅
	m_pAnimController->AdvanceTime(m_fAnimDelta, NULL);
	m_fAnimDelta = 0.0f;		//애니메이션을 진행시켰기 때문에 delta 량은 0 으로....

	//각 본에 행렬 대입
	MAP_BONETRANSFORM::iterator iterBone;
	for (iterBone = m_mapBoneTransform.begin(); iterBone != m_mapBoneTransform.end(); ++iterBone)
		iterBone->first->pBoneTransform = iterBone->second;

	for (iterBone = m_mapApplyBoneTransform.begin(); iterBone != m_mapApplyBoneTransform.end(); ++iterBone)
		iterBone->first->pApplyTransform = iterBone->second;

	m_pSkinnedMesh->RenderBoneName(pCam, pTransform);
}

//Animation 이름으로 플레이
void cSkinnedAnimation::Play(std::string animName, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = true;

	MAP_ANIMSET::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end())
	{
		//크로스 페이드 타임 기억
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(find->second);
	}
}


//Animation 인덱스로 플레이
void cSkinnedAnimation::Play(int animIndex, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = true;

	if (animIndex < this->m_AnimNum)
	{
		//크로스 페이드 타임 기억
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(m_vecAnimSet[animIndex]);
	}
}

void cSkinnedAnimation::Play(LPD3DXANIMATIONSET animSet, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = true;

	//크로스 페이드 타임 기억
	m_fCrossFadeTime = crossFadeTime;
	m_fLeftCrossFadeTime = crossFadeTime;


	this->SetAnimation(animSet);

}


void cSkinnedAnimation::PlayOneShot(std::string animName, float inCrossFadeTime, float outCrossFadeTime)
{
	m_bPlay = true;
	m_bLoop = false;

	MAP_ANIMSET::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end())
	{

		//현재 Animaiton 을 기억한다.
		this->m_pPrevPlayAnimationSet = this->m_pNowPlayAnimationSet;

		//크로스 페이드 타임 기억
		m_fCrossFadeTime = inCrossFadeTime;
		m_fLeftCrossFadeTime = inCrossFadeTime;

		//나갈때 크로스페이드 타입 기억
		m_fOutCrossFadeTime = outCrossFadeTime;

		this->SetAnimation(find->second);
	}
}


void cSkinnedAnimation::PlayOneShotAfterHold(std::string animName, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = false;

	MAP_ANIMSET::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end())
	{

		//돌아갈 Animation 은 없다
		this->m_pPrevPlayAnimationSet = NULL;

		//크로스 페이드 타임 기억
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(find->second);
	}
}

void cSkinnedAnimation::SetPlaySpeed(float speed)
{
	m_pAnimController->SetTrackSpeed(0, speed);
}

void cSkinnedAnimation::AddBoneTransform(std::string boneName, cTransform* pTransform)
{
	BONE* pBone = this->m_pSkinnedMesh->GetFineBONE(boneName);
	this->m_mapBoneTransform.insert(std::make_pair(pBone, pTransform));
}

void cSkinnedAnimation::AddApplyTransform(std::string boneName, cTransform* pTransform)
{
	BONE* pBone = this->m_pSkinnedMesh->GetFineBONE(boneName);
	this->m_mapApplyBoneTransform.insert(std::make_pair(pBone, pTransform));
}

void cSkinnedAnimation::RemoveBoneTransform(std::string boneName)
{
	BONE* pBone = this->m_pSkinnedMesh->GetFineBONE(boneName);
	MAP_BONETRANSFORM::iterator boneIter = m_mapBoneTransform.find(pBone);
	if (boneIter != m_mapBoneTransform.end())
	{
		m_mapBoneTransform.erase(boneIter);
	}

}


void cSkinnedAnimation::RemoveApplyTransform(std::string boneName)
{
	BONE* pBone = this->m_pSkinnedMesh->GetFineBONE(boneName);
	MAP_BONETRANSFORM::iterator boneIter = m_mapApplyBoneTransform.find(pBone);
	if (boneIter != m_mapApplyBoneTransform.end())
	{
		m_mapApplyBoneTransform.erase(boneIter);
	}
}


/////////////////////////////////////////////////////////////////////////////

//Animation 을 바꾼다.
void cSkinnedAnimation::SetAnimation(LPD3DXANIMATIONSET animSet)
{
	//이미 플레이되고 있는 Animation 이라면 실행이 되지 않는다.
	if (this->m_pNowPlayAnimationSet != NULL &&
		animSet == this->m_pNowPlayAnimationSet)
		return;

	//크로스 페이드가 존재한다면..
	if (this->m_fCrossFadeTime > 0.0f)
	{
		//현제 Animation 을 1 번Track 으로 셋팅
		m_pAnimController->SetTrackAnimationSet(1, m_pNowPlayAnimationSet);
		m_pAnimController->SetTrackPosition(1, m_Track_Desc_0.Position);	//이전에 플레이 되던 위치로 셋팅
		m_pAnimController->SetTrackEnable(1, true); //1 번 Track 활성화
		m_pAnimController->SetTrackWeight(1, 1.0f); //1 번 Track 가중치
		m_pAnimController->SetTrackSpeed(1, m_Track_Desc_0.Speed);		//속도 

		//지금셋팅되는 Animation 을 0 번Track 으로 셋팅
		m_pNowPlayAnimationSet = animSet;
		m_pAnimController->SetTrackAnimationSet(0, animSet);
		m_pAnimController->SetTrackPosition(0, 0.0f);
		m_pAnimController->SetTrackWeight(0, 0.0f);	//가중치는 0 으로 

		//현재 플레이 되고 있는 AnimationSet 갱신
		this->m_pNowPlayAnimationSet = animSet;
	}

	//크로스 페이드가 없다면...
	else
	{
		//Track 의 포지션을 맨압으로 돌린다.
		this->m_pAnimController->SetTrackPosition(
			0,			//Track 번호 
			0.0			//PlayPosition
			);


		//매개변수로 받은 AnimationSet 으로 Animation 0 번 Track에 플레이
		this->m_pAnimController->SetTrackAnimationSet(0, animSet);

		//현재 플레이 되고 있는 AnimationSet 갱신
		this->m_pNowPlayAnimationSet = animSet;
	}
}
