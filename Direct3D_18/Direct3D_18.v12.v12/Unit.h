#pragma once
#include <map>


class State;
class cTransform;
class cXMesh_Skinned;
class cSkinnedAnimation;
class cLight;
class State;
class cCamera;
class cNode;
class cBoundBox;
class cBoundSphere;
class InputHandler;


class Unit
{
protected:

	//�밡�� ��ġ
	D3DXVECTOR3 headPos;

	//�밡�� Trans
	cTransform*	 pHeadTrans;

	//������ �浹 ��
	cBoundSphere* CollisionSphere;

	//������ ���� �����ϴ� ��?
	cBoundSphere* DetectSphere;

	//���� ���� ���� ��Ÿ���� Class
	State* m_pCurState;

	// ���� �ִϸ��̼� �̸��� �����ϱ� ���� ����
	string m_Animation_Name;

	//��Ų�� �ִϸ��̼� ���� 
	cSkinnedAnimation*		pSkinnedAni;

	// ������ ���� ��ġ�� ��� ���� Ʈ���� ��
	cTransform*			pSkinnedTrans;

	// ������ ���� �����ϱ� ���� light ��
	std::vector<cLight*>	lights;

	// ������ XFile�� ���� ����
	string m_FilePath;



	std::map< string , State* > m_MState;
	std::map< string, State* >::iterator m_MiState;
	// ���� �����̴� ���´�

public:
	Unit() {};
	~Unit() {};

	virtual HRESULT Init() { return S_OK;  };
	virtual void Update(float timeDelta) = 0;
	virtual void Release() = 0;
	virtual void Render();

	// ó�� ������ State Map�� State�� �־��ִ� �Լ���. state �̸��� ���� �ִϸ��̼ǵ� �� �̸��� �°� �ٲ�.
	// �� ) 'RUN' State�� ���� ������, 'RUN' �ִϸ��̼� �����Ͱ� ����.
	virtual void InitAnimation() = 0;
	//�� �ʱ�ȭ ���ִ� �Լ�,
	virtual void InitLight() = 0;

	virtual void StateChange(string StateName, float ChangeTime);  // State�� �ٲ���.
	virtual void StatePlayChange(string StateName, float ChangeTime);  // State �ٲٰ� �׿� �´� �ִϸ��̼� Play
	virtual void StateOneShotChange(string StateName, float ChangeTime);  // State �ٲٰ� �׿� �´� �ִϸ��̼� OneShotPlay
	virtual void StateOneShotHoldChange(string StateName, float ChangeTime);  // State �ٲٰ� �׿� �´� �ִϸ��̼� OneShotPlayandHold

	virtual void LerpMoveControll(float timeDelta, cNode * PreNode, cNode* NextNode);
	virtual State* GetState() { return m_pCurState; }
	virtual cSkinnedAnimation* GetSkinned() { return pSkinnedAni;  }
	virtual cTransform* GetTrans() { return pSkinnedTrans; }
	virtual string getCurAniName() { return m_Animation_Name; }
    D3DXVECTOR3 getHeadPos() 
	{
		headPos.y -= 0.15;
		return headPos;
	}
	virtual void AttachToCamera(cTransform* camera);
	virtual void CameraAttachToUnit(cTransform* camera);

};


class PigEnemy : public Unit
{

public :
	PigEnemy(std::string filePath, D3DXVECTOR3 pos);

	~PigEnemy() {};

//	HRESULT Init();
	virtual void Update(float timeDelta) override;
	virtual void Release() override;


	virtual void InitAnimation() override;
	virtual void InitLight() override;

};

class Player : public Unit
{
private:
	InputHandler* input;

public:
	Player(std::string filePath, D3DXVECTOR3 pos);
	~Player() {};

//	HRESULT Init();
	virtual void Update(float timeDelta) override;
	virtual void Release() override;
	virtual void PlayerControll(float timeDelta);
	virtual bool checkEnterWalking();

	virtual void InitAnimation() override;
	virtual void InitLight() override;
	virtual void SitDown();

};

