#include "stdafx.h"
#include "cXMesh_Skinned.h"
#include "cCamera.h"

char szTemp[128];
LPD3DXEFFECT cXMesh_Skinned::sSkinnedMeshEffect = NULL;


cXMesh_Skinned::cXMesh_Skinned()
	:m_pNowPlayAnimationSet(NULL), m_dwWorkingPaletteSize(0), m_pmWorkingPalette(NULL), m_pSkinnedEffect(NULL)
{
	//Skinned Effect �ε�
	if (sSkinnedMeshEffect == NULL)
		sSkinnedMeshEffect = RESOURCE_FX->GetResource("../../Resources/Shaders/SkinnedMesh.fx");
}


cXMesh_Skinned::~cXMesh_Skinned()
{
}


HRESULT cXMesh_Skinned::Init(std::string filePath, const D3DXMATRIXA16* pmatCorrection)
{
	//��������� ���Դٸ�...
	if (pmatCorrection)
		m_matCorrection = *pmatCorrection;
	else
		D3DXMatrixIdentity(&m_matCorrection);


	m_pSkinnedEffect = sSkinnedMeshEffect;


	//��Ų��޽��� ���� ���� ������ �ε������ �����ǵ� BONE_HIERARCHY Ŭ���� ��ü
	BONE_HIERARCHY boneHierachy;
	boneHierachy.SetLoadPath(filePath);
	boneHierachy.SetSkinnedMesh(this);


	//���������� �Ǿ��ִ� XFile �޽� �ε�....
	if (FAILED(D3DXLoadMeshHierarchyFromX(
		filePath.c_str(),
		D3DXMESH_MANAGED,
		Device,
		&boneHierachy,					//���� ������ �ε� ����� �����ǵ� ��ü�� �ѱ��.
		NULL,
		&m_pRootbone,					//���� ��Ʈ ��������
		&m_pAnimController				//���� �ִϸ��̼� ��Ʈ�ѷ�
		))) return E_FAIL;

	//�� ��Ʈ���� ������ ����
	InitBoneMatrixPointer((BONE*)m_pRootbone);

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

	return S_OK;
}


void cXMesh_Skinned::Release()
{
	//�ҷ��� AnimationSet ����
	for (int i = 0; i < this->m_vecAnimSet.size(); i++)
		SAFE_RELEASE(m_vecAnimSet[i]);

	//���ϸ��̼� ��Ʈ�ѷ� ����
	SAFE_RELEASE(m_pAnimController);

	//��Ų��޽��� ���� ���� ������ �ε������ �����ǵ� BONE_HIERARCHY Ŭ���� ��ü
	BONE_HIERARCHY boneHierachy;	//<- ���� ���� ����� �˰� �ִ�.
	D3DXFrameDestroy(m_pRootbone, &boneHierachy);

}


void cXMesh_Skinned::Update(float timeDelta)
{
	//0 �� Track ������ ��´�
	m_pAnimController->GetTrackDesc(0, &m_Track_Desc_0);

	// m_pNowPlayAnimationSet->GetPeriod() Animation �� �ѽð��� ��´�.
	//���� Animation ���� Factor �� ����
	m_AnimationPlayFactor = m_Track_Desc_0.Position / m_pNowPlayAnimationSet->GetPeriod();

	//�������� �����ߴٸ�...
	if (m_AnimationPlayFactor >= 1.0)
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
		m_pAnimController->AdvanceTime(timeDelta, NULL);
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

void cXMesh_Skinned::Render(const cTransform* pTrans)
{

	D3DXMATRIXA16 matFinal;

	//Transform �� ���Դٸ�...
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

	//SkinnedEffect WorldMatrix ����
	m_pSkinnedEffect->SetMatrix("matWorld", &matFinal);


	//��� ������Ʈ 
	UpdateMatrices((BONE*)m_pRootbone, &matFinal);

	//��Ʈ ������ Ÿ�� ����.
	RenderBone((BONE*)m_pRootbone);
}

//���� �����ش�.
void cXMesh_Skinned::RenderBoneName(cCamera* pCam, cTransform* transform)
{
	D3DXMATRIXA16 matFinal;

	//Transform �� ���Դٸ�...
	if (transform != NULL)
	{
		D3DXMATRIXA16 matWorld = transform->GetFinalMatrix();
		matFinal = m_matCorrection * matWorld;
	}
	else
	{
		matFinal = m_matCorrection;
	}



	//��� ������Ʈ 
	UpdateMatrices((BONE*)m_pRootbone, &matFinal);

	//��Ʈ ������ Ÿ�� ����.
	RenderChildBoneName(pCam, (BONE*)m_pRootbone, NULL);
}

//Animation �̸����� �÷���
void cXMesh_Skinned::Play(std::string animName, float crossFadeTime)
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
void cXMesh_Skinned::Play(int animIndex, float crossFadeTime)
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


void cXMesh_Skinned::PlayOneShot(std::string animName, float inCrossFadeTime, float outCrossFadeTime)
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


void cXMesh_Skinned::PlayOneShotAfterHold(std::string animName, float crossFadeTime)
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

//Animation ������ ����Ѵ�.
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

//���̸����� ��ã�´�.
BONE* cXMesh_Skinned::GetFineBONE(std::string boneName)
{
	return (BONE*)D3DXFrameFind(m_pRootbone, boneName.c_str());
}


/////////////////////////////////////////////////////////////////////////////////////

//���� �׸���.
void cXMesh_Skinned::RenderBone(BONE* pBone)
{
	if (pBone == NULL) return;

	//���� �޽� �����̳ʰ� �����Ѵٸ� �׷����Ѵ�.
	if (pBone->pMeshContainer)
	{
		//���� �ִ� �޽� �����̳ʴ� BONE_MESH �̴�
		BONE_MESH* pBoneMesh = (BONE_MESH*)pBone->pMeshContainer;

		//�� �ĺ���̼� ������  
		if (pBoneMesh->BufBoneCombos != NULL)
		{
			//�ش� ���� �ĺ���̼� ������ ��´�.
			LPD3DXBONECOMBINATION pBoneComb =
				(LPD3DXBONECOMBINATION)(pBoneMesh->BufBoneCombos->GetBufferPointer());

			//���� �����ִ� �޽��� ����°����� �Ӽ��׷���� ����
			for (DWORD i = 0; i < pBoneMesh->NumAttributesGroup; i++)
			{
				//�ش� �Ӽ��� �ȷ�Ʈ ��Ʈ�� ����ŭ ���鼭 �۾��� �ȷ�Ʈ ��� ����
				for (DWORD palEntry = 0; palEntry < pBoneMesh->NumPaletteEntries; palEntry++)
				{
					//����Ǵ� ��� ID �� ��´�
					DWORD dwMatrixIndex = pBoneComb[i].BoneId[palEntry];

					//��� �ε����� ��ȿ�ϴٸ�...
					if (dwMatrixIndex != UINT_MAX)
					{
						//�۾� �޷��� �����.
						D3DXMatrixMultiply(&m_pmWorkingPalette[palEntry],
							&(pBoneMesh->pBoneOffsetMatices[dwMatrixIndex]),
							pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
					}
				}

				//������ ���É� �۾������ Effect �ȷ����� �����Ѵ�.
				m_pSkinnedEffect->SetMatrixArray(
					"amPalette", m_pmWorkingPalette, pBoneMesh->NumPaletteEntries);

				//����Ǵ� ������ ���ִ� ����� �� ���� �ִ� �����  -1 
				m_pSkinnedEffect->SetInt("CurNumBones", pBoneMesh->MaxNumFaceInfls - 1);

				//���͸��� �ε���
				DWORD materialIndex = pBoneComb[i].AttribId;

				//�ؽ��� ����
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


	//�̿� ���� �����Ѵٸ�...
	if (pBone->pFrameSibling)
		RenderBone((BONE*)pBone->pFrameSibling);

	//�ڽ� ���� �����Ѵٸ�...
	if (pBone->pFrameFirstChild)
		RenderBone((BONE*)pBone->pFrameFirstChild);
}


//���̸��� ����Ѵ�.
void cXMesh_Skinned::RenderChildBoneName(cCamera* pCam, BONE* pBone, D3DXVECTOR3* parentPos)
{
	if (pBone == NULL) return;

	//���� Transform  �׷�
	if (pBone->pBoneTransform != NULL)
		pBone->pBoneTransform->RenderGimozo();

	//����ġ
	D3DXVECTOR3 bonePos = D3DXVECTOR3(
		pBone->CombinedTransformationMatrix._41,
		pBone->CombinedTransformationMatrix._42,
		pBone->CombinedTransformationMatrix._43);

	sprintf(szTemp, "%s", pBone->Name);

	DWORD color = 0xff808000;
	DWORD textColor = 0xffffff00;


	//��Ʈ���̶��.
	if (pBone == m_pRootbone)
	{
		color = 0xff808080;
		textColor = 0xffffffff;
		//����ġ�� Gizmo
		GIZMO_MGR->WireSphere(bonePos, 0.2f, color);
	}


	//����ġ�� Gizmo
	GIZMO_MGR->WireSphere(bonePos, 0.1f, color);


	//�θ���ġ���� ����ġ�� ��
	if (parentPos != NULL)
		GIZMO_MGR->Line(bonePos, *parentPos, color);

	//����ũ�� ��ġ
	D3DXVECTOR2 boneScreenPos;
	if (pCam->GetWorldPosToScreenPos(&boneScreenPos, &bonePos))
	{
		DXFONT_MGR->PrintText(szTemp, (int)boneScreenPos.x, (int)boneScreenPos.y, textColor);
	}


	//�̿� ���� �����Ѵٸ�...
	if (pBone->pFrameSibling)
		RenderChildBoneName(pCam, (BONE*)pBone->pFrameSibling, parentPos);

	//�ڽ� ���� �����Ѵٸ�...
	if (pBone->pFrameFirstChild)
		RenderChildBoneName(pCam, (BONE*)pBone->pFrameFirstChild, &bonePos);

}

//�� ����� �����Ѵ�.
void cXMesh_Skinned::InitBoneMatrixPointer(BONE* pBone)
{
	if (pBone == NULL) return;

	//�ش� �������ӿ� �޽� �����̳� �� �����Ѵٸ�...
	if (pBone->pMeshContainer)
	{
		//�޽� �����̳ʸ� BONE_MESH ������ ����ȯ
		BONE_MESH* pBoneMesh = (BONE_MESH*)pBone->pMeshContainer;

		//�޽��� ��Ű�� ������ �ִٸ�
		if (pBoneMesh->pSkinInfo)
		{
			//�ش� ��Ų�� ����Ǵ� ���� ������ŭ
			int numBones = pBoneMesh->pSkinInfo->GetNumBones();

			//Matrix ������ �Ҵ�
			pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIXA16*[numBones];

			//����Ǵ� �� �������� ��Ʈ������ ã�� �ش� ���������� ��� �����͸� ������.
			for (int i = 0; i < numBones; i++)
			{
				//�ش� ��Ų�� ����Ǵ� i ��°�� ���̸����� �ش� �� �������� ���� ������ ã�� ����
				BONE* pB = (BONE*)D3DXFrameFind(m_pRootbone, pBoneMesh->pSkinInfo->GetBoneName(i));

				//ã�Ҵٸ�
				if (pB != NULL)
					pBoneMesh->ppBoneMatrixPtrs[i] = &pB->CombinedTransformationMatrix;

				else
					pBoneMesh->ppBoneMatrixPtrs[i] = NULL;
			}

		}
	}

	//�̿� ���� �����Ѵٸ�...
	if (pBone->pFrameSibling)
		InitBoneMatrixPointer((BONE*)pBone->pFrameSibling);

	//�ڽ� ���� �����Ѵٸ�...
	if (pBone->pFrameFirstChild)
		InitBoneMatrixPointer((BONE*)pBone->pFrameFirstChild);
}

//����� �� ������Ʈ�Ѵ�.
void cXMesh_Skinned::UpdateMatrices(BONE* pBone, D3DXMATRIXA16* pParentMatrix)
{
	if (pBone == NULL) return;

	//�θ� ����� �ִٸ�..
	if (pParentMatrix != NULL)
	{
		D3DXMATRIXA16 matTransformation = pBone->TransformationMatrix;


		//���� Transform �� ���� �ִٸ�...
		if (pBone->pApplyTransform != NULL)
		{
			D3DXMATRIXA16 pApplyMatrix = pBone->pApplyTransform->GetWorldRotateMatrix();
			matTransformation = matTransformation * pApplyMatrix;
			pBone->pApplyTransform = NULL;
		}


		//pBone->CombinedTransformationMatrix = pBone->TransformationMatrix * *(pParentMatrix);
		//pBone �� ��� �θ������ ���Ͽ� pBone �� ���� ����� ���Ѵ�.
		D3DXMatrixMultiply(
			&pBone->CombinedTransformationMatrix,
			&matTransformation,
			pParentMatrix);
	}

	//�θ� ����� ���ٸ�..
	else
	{
		//�ڽ��� ����� ���� ����� �ȴ�.
		pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
	}

	//�� Transform �� ���� �ִٸ�....
	if (pBone->pBoneTransform != NULL)
	{
		pBone->pBoneTransform->SetWorldMatrix(
			pBone->CombinedTransformationMatrix);

		pBone->pBoneTransform->SetScale(1, 1, 1);
		pBone->pBoneTransform = NULL;
	}

	//�̿� ���� �����Ѵٸ�...
	if (pBone->pFrameSibling)
		UpdateMatrices((BONE*)pBone->pFrameSibling, pParentMatrix);	//�̿��� ���������̱� ������ �θ� ���� ���� �Ű����� ���� �θ��� ����� �ѱ�� �ȴ�.

	//�ڽ� ���� �����Ѵٸ�...
	if (pBone->pFrameFirstChild)
		UpdateMatrices((BONE*)pBone->pFrameFirstChild, &pBone->CombinedTransformationMatrix);

}

//Animation �� �ٲ۴�.
void cXMesh_Skinned::SetAnimation(LPD3DXANIMATIONSET animSet)
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


////////////////////////////////////////////////////////////////////////////////

void cXMesh_Skinned::SetCamera(const cCamera* Camera)
{
	D3DXMATRIXA16 matViewProj = Camera->GetViewProjectionMatrix();

	sSkinnedMeshEffect->SetMatrix("matViewProjection", &matViewProj);

	D3DXVECTOR3 vEyePos = Camera->GetWorldPosition();

	sSkinnedMeshEffect->SetVector("vEyePos", &D3DXVECTOR4(vEyePos, 1));
}
