#pragma once

#include "cxmesh.h"

#include "BoneHierachy.h"		//�����̾� ��Ű
#include "cSkinnedAnimation.h"

class cTransform;
class cCamera;

class cXMesh_Skinned : public cXMesh
{
private:
	friend class BONE_HIERARCHY;							//�����̾� ��Ű�� ��ģ��....
	friend class cSkinnedAnimation;							//cSkinnedAnimation �� ��ģ��...
	typedef std::map< std::string, LPD3DXANIMATIONSET >			MAP_ANIMSET;
	typedef std::vector< LPD3DXANIMATIONSET >					VEC_ANIMSET;


public:
	static LPD3DXEFFECT				sSkinnedMeshEffect;				//StaticMesh �� �׸��� Effect


private:
	LPD3DXEFFECT				m_pSkinnedEffect;	//Skinned �޽� Effect;

	LPD3DXFRAME					m_pRootbone;		//Skinned �޽��� ��Ʈ ��
	D3DXMATRIXA16				m_matCorrection;	//�������

	DWORD						m_dwWorkingPaletteSize;		//��� �۾� �ȷ�Ʈ��
	D3DXMATRIX *				m_pmWorkingPalette;			//��� �۾� �ȷ�Ʈ 

	/////////////////////////////////////////////////////////////////////////////////

	LPD3DXANIMATIONCONTROLLER		m_pAnimController;			//Animation ��Ʈ�ѷ�
	UINT							m_AnimNum;					//Animation ����
	MAP_ANIMSET						m_mapAnimSet;				//Map ���� �Ǿ��ִ� AnimationSet
	VEC_ANIMSET						m_vecAnimSet;				//Vector �� �Ǿ��ִ� AnimationSet

	LPD3DXANIMATIONSET				m_pNowPlayAnimationSet;		//���� �÷��̵ǰ��ִ� AnimationSet
	D3DXTRACK_DESC					m_Track_Desc_0;				//0�� Track_Desc ( 0 �� Track �� ���� )


	bool							m_bPlay;					//���� �÷��� ���̴�?
	bool							m_bLoop;					//���� �÷��̴�?
	LPD3DXANIMATIONSET				m_pPrevPlayAnimationSet;	//OneShot �÷��̽� �ѹ� Animation �÷��̵ǰ� �ٽ� �ǵ��ư� Animaiton

	float							m_fCrossFadeTime;			//ũ�ν����̵� Ÿ��
	float							m_fLeftCrossFadeTime;		//����ũ�ν����̵� Ÿ��
	float							m_fOutCrossFadeTime;		//OneShot Animation ������ �ǵ��ư��� CrossFade Ÿ��
	double							m_AnimationPlayFactor;		//Animation ���� Factor ( 0 �̸� ó�� 1 �ϼ��� ������ )

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

	//Animation ������ ����Ѵ�.
	void ShowAnimationName(int x, int y);

	//�ش� �ε����� Animation Set �� ��´�.
	LPD3DXANIMATIONSET GetAnimSet(int index)
	{
		return m_vecAnimSet[index];
	}

	//���̸����� ��ã�´�.
	BONE* GetFineBONE(std::string boneName);

private:

	//�� ����� �����Ѵ�.
	void InitBoneMatrixPointer(BONE* pBone);

	//����� �� ������Ʈ�Ѵ�.
	void UpdateMatrices(BONE* pBone, D3DXMATRIXA16* pParentMatrix);

	//������Ʈ �� ���� ��ĵǷ� �޽��� �����Ѵ�.
	void UpdateSkinnedMesh(BONE* pBone);

	//���� �׸���.
	void RenderBone(BONE* pBone);

	//���̸��� ����Ѵ�.
	void RenderChildBoneName(cCamera* pCam, BONE* pBone, D3DXVECTOR3* parentPos);

	//Animation �� �ٲ۴�.
	void SetAnimation(LPD3DXANIMATIONSET animSet);

public:
	static void SetCamera(const cCamera* Camera);

};

