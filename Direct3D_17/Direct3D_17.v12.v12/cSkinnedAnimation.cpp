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

	//SKinned Mesh �� Animation �� �����Ѵ�.
	m_pSkinnedMesh->m_pAnimController->CloneAnimationController(
		m_pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		m_pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		m_pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		m_pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);

	//Animation ������ ��´�.
	m_AnimNum = m_pAnimController->GetNumAnimationSets();

	for (UINT i = 0; i < m_AnimNum; i++)
	{
		//���ϸ��̼� �� ����
		LPD3DXANIMATIONSET animSet;

		//i ��° Animation �� ������ ��´�.
		m_pAnimController->GetAnimationSet(i, &animSet);

		//AnimationSet Vector �� ����
		this->m_vecAnimSet.push_back(animSet);

		//AnimationSet Map �� ����
		this->m_mapAnimSet.insert(std::make_pair(
			animSet->GetName(),
			animSet));
	}

	//�ϴ� ù��° Animation ���� ����
	this->Play(0);
}

void cSkinnedAnimation::Release()
{
	SAFE_RELEASE(m_pAnimController);
}

void cSkinnedAnimation::Update(float timeDelta)
{
	//0 �� Track ������ ��´�
	m_pAnimController->GetTrackDesc(0, &m_Track_Desc_0);

	// m_pNowPlayAnimationSet->GetPeriod() Animation �� �ѽð��� ��´�.
	//���� Animation ���� Factor �� ����
	m_AnimationPlayFactor = m_Track_Desc_0.Position / m_pNowPlayAnimationSet->GetPeriod();


	//�������� �����ߴٸ�...
	if (m_AnimationPlayFactor >= 0.95f)
	{
		if (this->m_bLoop == false){

			//���ư� Animation �� �ִٸ�..
			if (this->m_pPrevPlayAnimationSet != NULL)
			{
				m_fCrossFadeTime = m_fOutCrossFadeTime;
				m_fLeftCrossFadeTime = m_fOutCrossFadeTime;
				m_bLoop = true;
				this->SetAnimation(m_pPrevPlayAnimationSet);
				this->m_pPrevPlayAnimationSet = NULL;
			}

			//���ư� Animation �� ���ٸ�.
			else
			{
				this->Stop();
			}
		}
	}

	m_AnimationPlayFactor = m_AnimationPlayFactor - (int)m_AnimationPlayFactor;	//�����κ� �Ȼ����....


	if (m_bPlay)
	{
		//�ִϸ��̼��� �����Ŵ
		//m_pAnimController->AdvanceTime( timeDelta, NULL );
		m_fAnimDelta = timeDelta;
	}
	else
	{
		m_fAnimDelta = 0.0f;
	}


	//ũ�ν� ���̵尡 �������̶��..
	if (m_fLeftCrossFadeTime > 0.0f)
	{
		//���� ũ�ν����̵� �ð� ����
		this->m_fLeftCrossFadeTime -= timeDelta;

		//ũ�ν����̵� ��������.
		if (m_fLeftCrossFadeTime <= 0.0f)
		{
			m_pAnimController->SetTrackWeight(0, 1);
			m_pAnimController->SetTrackEnable(1, false);
		}

		else
		{
			float w1 = (m_fLeftCrossFadeTime / m_fCrossFadeTime);		//1�� Track ����ġ
			float w0 = 1.0f - w1;										//0�� Track ����ġ

			//����ġ ����
			m_pAnimController->SetTrackWeight(0, w0);
			m_pAnimController->SetTrackWeight(1, w1);
		}
	}

}


void cSkinnedAnimation::Render(cTransform* pTransform)
{
	//���� �ڽ��� Animation ������ ����
	m_pAnimController->AdvanceTime(m_fAnimDelta, NULL);
	m_fAnimDelta = 0.0f;		//�ִϸ��̼��� ������ױ� ������ delta ���� 0 ����....

	//�� ���� ��� ����
	MAP_BONETRANSFORM::iterator iterBone;
	for (iterBone = m_mapBoneTransform.begin(); iterBone != m_mapBoneTransform.end(); ++iterBone)
		iterBone->first->pBoneTransform = iterBone->second;

	for (iterBone = m_mapApplyBoneTransform.begin(); iterBone != m_mapApplyBoneTransform.end(); ++iterBone)
		iterBone->first->pApplyTransform = iterBone->second;

	m_pSkinnedMesh->Render(pTransform);
}

void cSkinnedAnimation::RenderBoneName(cCamera* pCam, cTransform* pTransform)
{
	//���� �ڽ��� Animation ������ ����
	m_pAnimController->AdvanceTime(m_fAnimDelta, NULL);
	m_fAnimDelta = 0.0f;		//�ִϸ��̼��� ������ױ� ������ delta ���� 0 ����....

	//�� ���� ��� ����
	MAP_BONETRANSFORM::iterator iterBone;
	for (iterBone = m_mapBoneTransform.begin(); iterBone != m_mapBoneTransform.end(); ++iterBone)
		iterBone->first->pBoneTransform = iterBone->second;

	for (iterBone = m_mapApplyBoneTransform.begin(); iterBone != m_mapApplyBoneTransform.end(); ++iterBone)
		iterBone->first->pApplyTransform = iterBone->second;

	m_pSkinnedMesh->RenderBoneName(pCam, pTransform);
}

//Animation �̸����� �÷���
void cSkinnedAnimation::Play(std::string animName, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = true;

	MAP_ANIMSET::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end())
	{
		//ũ�ν� ���̵� Ÿ�� ���
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(find->second);
	}
}


//Animation �ε����� �÷���
void cSkinnedAnimation::Play(int animIndex, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = true;

	if (animIndex < this->m_AnimNum)
	{
		//ũ�ν� ���̵� Ÿ�� ���
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(m_vecAnimSet[animIndex]);
	}
}

void cSkinnedAnimation::Play(LPD3DXANIMATIONSET animSet, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = true;

	//ũ�ν� ���̵� Ÿ�� ���
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

		//���� Animaiton �� ����Ѵ�.
		this->m_pPrevPlayAnimationSet = this->m_pNowPlayAnimationSet;

		//ũ�ν� ���̵� Ÿ�� ���
		m_fCrossFadeTime = inCrossFadeTime;
		m_fLeftCrossFadeTime = inCrossFadeTime;

		//������ ũ�ν����̵� Ÿ�� ���
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

		//���ư� Animation �� ����
		this->m_pPrevPlayAnimationSet = NULL;

		//ũ�ν� ���̵� Ÿ�� ���
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

//Animation �� �ٲ۴�.
void cSkinnedAnimation::SetAnimation(LPD3DXANIMATIONSET animSet)
{
	//�̹� �÷��̵ǰ� �ִ� Animation �̶�� ������ ���� �ʴ´�.
	if (this->m_pNowPlayAnimationSet != NULL &&
		animSet == this->m_pNowPlayAnimationSet)
		return;

	//ũ�ν� ���̵尡 �����Ѵٸ�..
	if (this->m_fCrossFadeTime > 0.0f)
	{
		//���� Animation �� 1 ��Track ���� ����
		m_pAnimController->SetTrackAnimationSet(1, m_pNowPlayAnimationSet);
		m_pAnimController->SetTrackPosition(1, m_Track_Desc_0.Position);	//������ �÷��� �Ǵ� ��ġ�� ����
		m_pAnimController->SetTrackEnable(1, true); //1 �� Track Ȱ��ȭ
		m_pAnimController->SetTrackWeight(1, 1.0f); //1 �� Track ����ġ
		m_pAnimController->SetTrackSpeed(1, m_Track_Desc_0.Speed);		//�ӵ� 

		//���ݼ��õǴ� Animation �� 0 ��Track ���� ����
		m_pNowPlayAnimationSet = animSet;
		m_pAnimController->SetTrackAnimationSet(0, animSet);
		m_pAnimController->SetTrackPosition(0, 0.0f);
		m_pAnimController->SetTrackWeight(0, 0.0f);	//����ġ�� 0 ���� 

		//���� �÷��� �ǰ� �ִ� AnimationSet ����
		this->m_pNowPlayAnimationSet = animSet;
	}

	//ũ�ν� ���̵尡 ���ٸ�...
	else
	{
		//Track �� �������� �Ǿ����� ������.
		this->m_pAnimController->SetTrackPosition(
			0,			//Track ��ȣ 
			0.0			//PlayPosition
			);


		//�Ű������� ���� AnimationSet ���� Animation 0 �� Track�� �÷���
		this->m_pAnimController->SetTrackAnimationSet(0, animSet);

		//���� �÷��� �ǰ� �ִ� AnimationSet ����
		this->m_pNowPlayAnimationSet = animSet;
	}
}
