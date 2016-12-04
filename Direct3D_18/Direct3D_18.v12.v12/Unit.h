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

	//대가리 위치
	D3DXVECTOR3 headPos;

	//대가리 Trans
	cTransform*	 pHeadTrans;

	//유닛의 충돌 구
	cBoundSphere* CollisionSphere;

	//유닛의 적을 감지하는 구?
	cBoundSphere* DetectSphere;

	//현재 상태 값을 나타내는 Class
	State* m_pCurState;

	// 현재 애니메이션 이름을 저장하기 위한 변수
	string m_Animation_Name;

	//스킨드 애니메이션 변수 
	cSkinnedAnimation*		pSkinnedAni;

	// 유닛의 현재 위치를 담기 위한 트랜스 폼
	cTransform*			pSkinnedTrans;

	// 유닛의 빛을 설정하기 위한 light 값
	std::vector<cLight*>	lights;

	// 유닛의 XFile을 담은 변수
	string m_FilePath;



	std::map< string , State* > m_MState;
	std::map< string, State* >::iterator m_MiState;
	// 현재 움직이는 상태니

public:
	Unit() {};
	~Unit() {};

	virtual HRESULT Init() { return S_OK;  };
	virtual void Update(float timeDelta) = 0;
	virtual void Release() = 0;
	virtual void Render();

	// 처음 설정시 State Map에 State를 넣어주는 함수임. state 이름에 따라 애니메이션도 그 이름에 맞게 바뀜.
	// 예 ) 'RUN' State를 만들어서 넣으면, 'RUN' 애니메이션 넣은것과 같음.
	virtual void InitAnimation() = 0;
	//빛 초기화 해주는 함수,
	virtual void InitLight() = 0;

	virtual void StateChange(string StateName, float ChangeTime);  // State만 바꿔줌.
	virtual void StatePlayChange(string StateName, float ChangeTime);  // State 바꾸고 그에 맞는 애니메이션 Play
	virtual void StateOneShotChange(string StateName, float ChangeTime);  // State 바꾸고 그에 맞는 애니메이션 OneShotPlay
	virtual void StateOneShotHoldChange(string StateName, float ChangeTime);  // State 바꾸고 그에 맞는 애니메이션 OneShotPlayandHold

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

