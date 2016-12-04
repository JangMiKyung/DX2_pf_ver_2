#include "stdafx.h"
#include "cXMesh_Skinned.h"
#include "cCamera.h"

char szTemp[128];
LPD3DXEFFECT cXMesh_Skinned::sSkinnedMeshEffect = NULL;


cXMesh_Skinned::cXMesh_Skinned()
	:m_pNowPlayAnimationSet(NULL), m_dwWorkingPaletteSize(0), m_pmWorkingPalette(NULL), m_pSkinnedEffect(NULL)
{
	//Skinned Effect 로딩
	if (sSkinnedMeshEffect == NULL)
		sSkinnedMeshEffect = RESOURCE_FX->GetResource("../../Resources/Shaders/SkinnedMesh.fx");
}


cXMesh_Skinned::~cXMesh_Skinned()
{
}


HRESULT cXMesh_Skinned::Init(std::string filePath, const D3DXMATRIXA16* pmatCorrection)
{
	//보정행렬이 들어왔다면...
	if (pmatCorrection)
		m_matCorrection = *pmatCorrection;
	else
		D3DXMatrixIdentity(&m_matCorrection);


	m_pSkinnedEffect = sSkinnedMeshEffect;


	//스킨드메쉬와 본의 계층 구조의 로딩방식이 재정의된 BONE_HIERARCHY 클래스 객체
	BONE_HIERARCHY boneHierachy;
	boneHierachy.SetLoadPath(filePath);
	boneHierachy.SetSkinnedMesh(this);


	//계층구조로 되어있는 XFile 메시 로딩....
	if (FAILED(D3DXLoadMeshHierarchyFromX(
		filePath.c_str(),
		D3DXMESH_MANAGED,
		Device,
		&boneHierachy,					//계층 구조의 로딩 방식이 재정의된 객체를 넘긴다.
		NULL,
		&m_pRootbone,					//얻어온 루트 본프레임
		&m_pAnimController				//얻어올 애니메이션 컨트롤러
		))) return E_FAIL;

	//본 매트릭스 포인터 생성
	InitBoneMatrixPointer((BONE*)m_pRootbone);

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

	return S_OK;
}


void cXMesh_Skinned::Release()
{
	//불러온 AnimationSet 해제
	for (int i = 0; i < this->m_vecAnimSet.size(); i++)
		SAFE_RELEASE(m_vecAnimSet[i]);

	//에니메이션 콘트롤러 해제
	SAFE_RELEASE(m_pAnimController);

	//스킨드메쉬와 본의 계층 구조의 로딩방식이 재정의된 BONE_HIERARCHY 클래스 객체
	BONE_HIERARCHY boneHierachy;	//<- 예는 해제 방법도 알고 있다.
	D3DXFrameDestroy(m_pRootbone, &boneHierachy);

}


void cXMesh_Skinned::Update(float timeDelta)
{
	//0 번 Track 정보를 얻는다
	m_pAnimController->GetTrackDesc(0, &m_Track_Desc_0);

	// m_pNowPlayAnimationSet->GetPeriod() Animation 의 총시간을 얻는다.
	//현재 Animation 진행 Factor 를 갱신
	m_AnimationPlayFactor = m_Track_Desc_0.Position / m_pNowPlayAnimationSet->GetPeriod();

	//마지막에 도달했다면...
	if (m_AnimationPlayFactor >= 1.0)
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
		m_pAnimController->AdvanceTime(timeDelta, NULL);
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

void cXMesh_Skinned::Render(const cTransform* pTrans)
{

	D3DXMATRIXA16 matFinal;

	//Transform 이 들어왔다면...
	if (pTrans != NULL)
	{
		D3DXMATRIXA16 matWorld = pTrans->GetFinalMatrix();
		matFinal = m_matCorrection * matWorld;
	}
	else
	{
		matFinal = m_matCorrection;
	}


	m_pSkinnedEffect->SetTechnique("SkinnedMesh");

	//SkinnedEffect WorldMatrix 적용
	m_pSkinnedEffect->SetMatrix("matWorld", &matFinal);


	//행렬 업데이트 
	UpdateMatrices((BONE*)m_pRootbone, &matFinal);

	//루트 본부터 타고 들어간다.
	RenderBone((BONE*)m_pRootbone);
}

//본을 보여준다.
void cXMesh_Skinned::RenderBoneName(cCamera* pCam, cTransform* transform)
{
	D3DXMATRIXA16 matFinal;

	//Transform 이 들어왔다면...
	if (transform != NULL)
	{
		D3DXMATRIXA16 matWorld = transform->GetFinalMatrix();
		matFinal = m_matCorrection * matWorld;
	}
	else
	{
		matFinal = m_matCorrection;
	}



	//행렬 업데이트 
	UpdateMatrices((BONE*)m_pRootbone, &matFinal);

	//루트 본부터 타고 들어간다.
	RenderChildBoneName(pCam, (BONE*)m_pRootbone, NULL);
}

//Animation 이름으로 플레이
void cXMesh_Skinned::Play(std::string animName, float crossFadeTime)
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
void cXMesh_Skinned::Play(int animIndex, float crossFadeTime)
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


void cXMesh_Skinned::PlayOneShot(std::string animName, float inCrossFadeTime, float outCrossFadeTime)
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


void cXMesh_Skinned::PlayOneShotAfterHold(std::string animName, float crossFadeTime)
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

//Animation 정보를 출력한다.
void cXMesh_Skinned::ShowAnimationName(int x, int y)
{
	int i = 0;
	for (i = 0; i < m_AnimNum; i++)
	{
		DXFONT_MGR->PrintText(
			this->m_vecAnimSet[i]->GetName(), x, y + i * 20, 0xffffffff);
	}
}

void cXMesh_Skinned::SetPlaySpeed(float speed)
{
	m_pAnimController->SetTrackSpeed(0, speed);
}

//본이름으로 본찾는다.
BONE* cXMesh_Skinned::GetFineBONE(std::string boneName)
{
	return (BONE*)D3DXFrameFind(m_pRootbone, boneName.c_str());
}


/////////////////////////////////////////////////////////////////////////////////////

//본을 그린다.
void cXMesh_Skinned::RenderBone(BONE* pBone)
{
	if (pBone == NULL) return;

	//본에 메쉬 컨테이너가 존재한다면 그려야한다.
	if (pBone->pMeshContainer)
	{
		//본에 있는 메쉬 컨테이너는 BONE_MESH 이다
		BONE_MESH* pBoneMesh = (BONE_MESH*)pBone->pMeshContainer;

		//본 컴비네이션 정보가  
		if (pBoneMesh->BufBoneCombos != NULL)
		{
			//해당 본의 컴비네이션 정보를 얻는다.
			LPD3DXBONECOMBINATION pBoneComb =
				(LPD3DXBONECOMBINATION)(pBoneMesh->BufBoneCombos->GetBufferPointer());

			//본에 물려있는 메쉬의 서브셋갯수을 속성그룹수와 같다
			for (DWORD i = 0; i < pBoneMesh->NumAttributesGroup; i++)
			{
				//해당 속성의 팔래트 엔트리 수만큼 돌면서 작업용 팔래트 행렬 갱신
				for (DWORD palEntry = 0; palEntry < pBoneMesh->NumPaletteEntries; palEntry++)
				{
					//적용되는 행렬 ID 를 얻는다
					DWORD dwMatrixIndex = pBoneComb[i].BoneId[palEntry];

					//행렬 인덱스가 유효하다면...
					if (dwMatrixIndex != UINT_MAX)
					{
						//작업 앵렬을 만든다.
						D3DXMatrixMultiply(&m_pmWorkingPalette[palEntry],
							&(pBoneMesh->pBoneOffsetMatices[dwMatrixIndex]),
							pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
					}
				}

				//위에서 셋팅됭 작업행렬을 Effect 팔래스에 적용한다.
				m_pSkinnedEffect->SetMatrixArray(
					"amPalette", m_pmWorkingPalette, pBoneMesh->NumPaletteEntries);

				//적용되는 정점의 본최대 영향수 를 대입 최대 영향수  -1 
				m_pSkinnedEffect->SetInt("CurNumBones", pBoneMesh->MaxNumFaceInfls - 1);

				//메터리얼 인덱스
				DWORD materialIndex = pBoneComb[i].AttribId;

				//텍스쳐 셋팅
				m_pSkinnedEffect->SetTexture("Diffuse_Tex", pBoneMesh->DiffuseTexs[materialIndex]);
				m_pSkinnedEffect->SetTexture("Specular_Tex", pBoneMesh->SpecularTexs[materialIndex]);
				m_pSkinnedEffect->SetTexture("Normal_Tex", pBoneMesh->NormalTexs[materialIndex]);
				m_pSkinnedEffect->SetTexture("Emission_Tex", pBoneMesh->EmissionTexs[materialIndex]);
				m_pSkinnedEffect->SetFloat("fSpecPower", pBoneMesh->Materials[materialIndex].Power);



				UINT uiPasses, uiPass;
				m_pSkinnedEffect->Begin(&uiPasses, 0);
				for (uiPass = 0; uiPass < uiPasses; ++uiPass)
				{
					m_pSkinnedEffect->BeginPass(uiPass);
					pBoneMesh->WorkingMesh->DrawSubset(i);
					m_pSkinnedEffect->EndPass();
				}
				m_pSkinnedEffect->End();
			}
		}
	}


	//이웃 본이 존재한다면...
	if (pBone->pFrameSibling)
		RenderBone((BONE*)pBone->pFrameSibling);

	//자식 본이 존재한다면...
	if (pBone->pFrameFirstChild)
		RenderBone((BONE*)pBone->pFrameFirstChild);
}


//본이름을 출력한다.
void cXMesh_Skinned::RenderChildBoneName(cCamera* pCam, BONE* pBone, D3DXVECTOR3* parentPos)
{
	if (pBone == NULL) return;

	//본의 Transform  그려
	if (pBone->pBoneTransform != NULL)
		pBone->pBoneTransform->RenderGimozo();

	//본위치
	D3DXVECTOR3 bonePos = D3DXVECTOR3(
		pBone->CombinedTransformationMatrix._41,
		pBone->CombinedTransformationMatrix._42,
		pBone->CombinedTransformationMatrix._43);

	sprintf(szTemp, "%s", pBone->Name);

	DWORD color = 0xff808000;
	DWORD textColor = 0xffffff00;


	//루트본이라면.
	if (pBone == m_pRootbone)
	{
		color = 0xff808080;
		textColor = 0xffffffff;
		//본위치에 Gizmo
		GIZMO_MGR->WireSphere(bonePos, 0.2f, color);
	}


	//본위치에 Gizmo
	GIZMO_MGR->WireSphere(bonePos, 0.1f, color);


	//부모위치에서 내위치의 선
	if (parentPos != NULL)
		GIZMO_MGR->Line(bonePos, *parentPos, color);

	//본스크린 위치
	D3DXVECTOR2 boneScreenPos;
	if (pCam->GetWorldPosToScreenPos(&boneScreenPos, &bonePos))
	{
		DXFONT_MGR->PrintText(szTemp, (int)boneScreenPos.x, (int)boneScreenPos.y, textColor);
	}


	//이웃 본이 존재한다면...
	if (pBone->pFrameSibling)
		RenderChildBoneName(pCam, (BONE*)pBone->pFrameSibling, parentPos);

	//자식 본이 존재한다면...
	if (pBone->pFrameFirstChild)
		RenderChildBoneName(pCam, (BONE*)pBone->pFrameFirstChild, &bonePos);

}

//본 행렬을 셋팅한다.
void cXMesh_Skinned::InitBoneMatrixPointer(BONE* pBone)
{
	if (pBone == NULL) return;

	//해당 본프레임에 메쉬 컨테이너 가 존재한다면...
	if (pBone->pMeshContainer)
	{
		//메쉬 컨테이너를 BONE_MESH 형으로 형변환
		BONE_MESH* pBoneMesh = (BONE_MESH*)pBone->pMeshContainer;

		//메쉬에 스키닝 정보가 있다면
		if (pBoneMesh->pSkinInfo)
		{
			//해당 스킨에 적용되는 본의 갯수만큼
			int numBones = pBoneMesh->pSkinInfo->GetNumBones();

			//Matrix 포인터 할당
			pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIXA16*[numBones];

			//적용되는 폰 프레임의 매트릭스를 찾아 해당 본프레임의 행렬 포인터를 물린다.
			for (int i = 0; i < numBones; i++)
			{
				//해당 스킨에 적용되는 i 번째의 본이름으로 해당 본 프레임을 루프 본부터 찾아 들어간가
				BONE* pB = (BONE*)D3DXFrameFind(m_pRootbone, pBoneMesh->pSkinInfo->GetBoneName(i));

				//찾았다면
				if (pB != NULL)
					pBoneMesh->ppBoneMatrixPtrs[i] = &pB->CombinedTransformationMatrix;

				else
					pBoneMesh->ppBoneMatrixPtrs[i] = NULL;
			}

		}
	}

	//이웃 본이 존재한다면...
	if (pBone->pFrameSibling)
		InitBoneMatrixPointer((BONE*)pBone->pFrameSibling);

	//자식 본이 존재한다면...
	if (pBone->pFrameFirstChild)
		InitBoneMatrixPointer((BONE*)pBone->pFrameFirstChild);
}

//본행렬 을 업데이트한다.
void cXMesh_Skinned::UpdateMatrices(BONE* pBone, D3DXMATRIXA16* pParentMatrix)
{
	if (pBone == NULL) return;

	//부모 행렬이 있다면..
	if (pParentMatrix != NULL)
	{
		D3DXMATRIXA16 matTransformation = pBone->TransformationMatrix;


		//적용 Transform 이 물려 있다면...
		if (pBone->pApplyTransform != NULL)
		{
			D3DXMATRIXA16 pApplyMatrix = pBone->pApplyTransform->GetWorldRotateMatrix();
			matTransformation = matTransformation * pApplyMatrix;
			pBone->pApplyTransform = NULL;
		}


		//pBone->CombinedTransformationMatrix = pBone->TransformationMatrix * *(pParentMatrix);
		//pBone 의 행렬 부모행렬을 곱하여 pBone 의 최종 행렬을 구한다.
		D3DXMatrixMultiply(
			&pBone->CombinedTransformationMatrix,
			&matTransformation,
			pParentMatrix);
	}

	//부모 행렬이 없다면..
	else
	{
		//자신의 행렬이 현재 행렬이 된다.
		pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
	}

	//본 Transform 이 물려 있다면....
	if (pBone->pBoneTransform != NULL)
	{
		pBone->pBoneTransform->SetWorldMatrix(
			pBone->CombinedTransformationMatrix);

		pBone->pBoneTransform->SetScale(1, 1, 1);
		pBone->pBoneTransform = NULL;
	}

	//이웃 본이 존재한다면...
	if (pBone->pFrameSibling)
		UpdateMatrices((BONE*)pBone->pFrameSibling, pParentMatrix);	//이웃은 같은레벨이기 때문에 부모가 같다 따라서 매개변로 받은 부모의 행렬을 넘기면 된다.

	//자식 본이 존재한다면...
	if (pBone->pFrameFirstChild)
		UpdateMatrices((BONE*)pBone->pFrameFirstChild, &pBone->CombinedTransformationMatrix);

}

//Animation 을 바꾼다.
void cXMesh_Skinned::SetAnimation(LPD3DXANIMATIONSET animSet)
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


////////////////////////////////////////////////////////////////////////////////

void cXMesh_Skinned::SetCamera(const cCamera* Camera)
{
	D3DXMATRIXA16 matViewProj = Camera->GetViewProjectionMatrix();

	sSkinnedMeshEffect->SetMatrix("matViewProjection", &matViewProj);

	D3DXVECTOR3 vEyePos = Camera->GetWorldPosition();

	sSkinnedMeshEffect->SetVector("vEyePos", &D3DXVECTOR4(vEyePos, 1));
}
