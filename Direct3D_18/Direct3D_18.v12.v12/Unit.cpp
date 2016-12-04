#include "stdafx.h"
#include "Unit.h"
#include "cTransform.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"
#include "State.h"
#include "cCamera.h"
#include "cNode.h"
#include"cBoundBox.h"
#include "cBoundSphere.h"


void Unit::StatePlayChange(string StateName, float ChangeTime)
{
	m_MiState = m_MState.find(StateName);
	 
	if (m_MiState != m_MState.end()   // Map �˻� �� ����,  
		&& m_Animation_Name != StateName)  // ���� ���°� �ְ��� �ϴ� ���¿� ���ٸ� ���� �ȹٲ㵵 ��.
	{
		m_Animation_Name = StateName;
		this->pSkinnedAni->Play(StateName, ChangeTime);
		m_pCurState = (*m_MiState).second;
	}
	
}

void Unit::StateOneShotChange(string StateName, float ChangeTime)
{
	m_MiState = m_MState.find(StateName);

	if (m_MiState != m_MState.end()   // Map �˻� �� ����,  
		&& m_Animation_Name != StateName)  // ���� ���°� �ְ��� �ϴ� ���¿� ���ٸ� ���� �ȹٲ㵵 ��.
	{
		m_Animation_Name = StateName;
		this->pSkinnedAni->PlayOneShot(StateName, ChangeTime, ChangeTime);

		m_pCurState = (*m_MiState).second;
	}
}

void Unit::StateOneShotHoldChange(string StateName, float ChangeTime)
{
	m_MiState = m_MState.find(StateName);

	if (m_MiState != m_MState.end()   // Map �˻� �� ����,  
		&& m_Animation_Name != StateName)  // ���� ���°� �ְ��� �ϴ� ���¿� ���ٸ� ���� �ȹٲ㵵 ��.
	{
		m_Animation_Name = StateName;
		this->pSkinnedAni->PlayOneShotAfterHold(StateName, ChangeTime);
		m_pCurState = (*m_MiState).second;
	}
//	this->pSkinnedAni->PlayOneShotAfterHold(StateName, ChangeTime);
}

void Unit::StateChange(string StateName, float ChangeTime)
{
	// State ���� �ٲ��ش�. 



}

// �������� �̿��Ͽ� �����̴� �Լ�. ���̳� AI�� ���� ����.
void Unit::LerpMoveControll(float timeDelta, cNode * PreNode, cNode* NextNode)
{
	m_pCurState->Move(timeDelta, PreNode->pos, NextNode->pos);
}


void Unit::AttachToCamera(cTransform * camera)
{    
	pSkinnedTrans->AttachTo(camera);
}

void Unit::CameraAttachToUnit(cTransform * camera)
{
	pSkinnedTrans->AddChild(camera);
}

void Unit::Render()
{
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->lights.size(); i++)
		matLights[i] = this->lights[i]->GetLightMatrix();

	//���̴��� ������ ����
	cXMesh_Skinned::sSkinnedMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Skinned::sSkinnedMeshEffect->SetInt("LightNum", this->lights.size());

	this->pSkinnedAni->Render(pSkinnedTrans);
	CollisionSphere->RenderGizmo(pSkinnedTrans);

	//GIZMO_MGR->Circle(pSkinnedTrans->GetWorldPosition(), 1);
	GIZMO_MGR->Circle(headPos, 1);

	RESOURCE_SKINNEDXMESH->GetResource(m_FilePath)->ShowAnimationName(0, 26);
}