#pragma once


#define RESET_POSITION	1
#define RESET_ROTATION	2
#define RESET_SCALE		4

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

//회전 허용축
#define SHAKE_X 1
#define SHAKE_Y 2
#define SHAKE_Z 4

__declspec(align(16))
class cTransform
{
protected:

	//축정보는 Union 으로 묵여 axis 배열로도 접근 가능하고, right, up, forward 로 도 접근가능하다.
	// ( 부모가 있으면 부모에대한 상태적인 로컬방향 없으면 월드방향 )
	union {

		struct{
			D3DXVECTOR3		axis[3];
		};

		struct{
			//바라보는 방향 값 아래의 3개는 반드시 직교화 되어있어야 한다.
			//항상정규화 되어있다.
			D3DXVECTOR3		right;
			D3DXVECTOR3		up;
			D3DXVECTOR3		forward;
		};
	};


	//위치값  ( 부모가 있으면 부모에대한 상태적인 로컬위치 없으면 월드 위치 )
	D3DXVECTOR3		position;

	//스케일 값 
	D3DXVECTOR3		scale;

	//로컬행렬	
	D3DXMATRIXA16	matLocal;				//부모를 기준으로한 상태적인 행렬 ( 만약 부모가 없다면 matFinal 이랑 같다 )

	//최종 행렬		( 부모가 있든없은 가장월드에 정직한행렬 )
	D3DXMATRIXA16	matFinal;


	//부모 참조 ( null 이면 부모가 없는애 )
	cTransform*		pParent;

	//자식 참조 ( null 이면 자식이 없는애 )
	cTransform*		pFirstChild;

	//형제 참조 ( null 이면 외아들 )
	cTransform*		pNextSibling;

	//transform 변화시 자동으로 업데이트가 될꺼니? ( 만약 이게 false 이면 사용자가 직접 UpdateTransform 변화후 호출 해주어야 한다 )
	bool			bAutoUpdate;


	//
	// Shake 관련
	//
	float ShakePosPower;			//현제 위치에 대한 흔들림 파워
	float ShakeRotPower;			//현제 회전에 대한 흔들림 파워

	float ShakePosRelease;			//위치 파워 초당 해제량
	float ShakeRotRelease;			//회전 파워 초당 해제량

	int ShakePosFlag;				//위치 허용축 플레그
	int ShakeRotFlag;				//회전 허용축 플레그



public:
	cTransform(void);
	~cTransform(void);

	//오토 업데이트 여부 셋팅
	void SetAutoUpdate(bool bAuto)
	{
		this->bAutoUpdate = bAuto;
	}

	//Transform 정보를 초기화
	void Reset(int resetFlag = -1);

	//특정 Child 를 내자식으로 붙인다.
	void AddChild(cTransform* pChild);

	//특정 Transform 에 붙는다.
	void AttachTo(cTransform* pParent);

	//부모와 안녕
	void ReleaseParent();

	//위치를 월드 좌표계로 셋팅한다. 
	void SetWorldPosition(float x, float y, float z);
	void SetWorldPosition(D3DXVECTOR3 pos);

	//위치를 로컬 좌표계로 셋팅한다.  ( 부모가 있는 경우 무모의 상태적인 위치 )
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(D3DXVECTOR3 pos);

	//자신의 축기준으로 이동 시킨다.
	void MovePositionSelf(float dx, float dy, float dz);
	void MovePositionSelf(D3DXVECTOR3 delta);

	//월드 기준으로 이동 시킨다.
	void MovePositionWorld(float dx, float dy, float dz);
	void MovePositionWorld(D3DXVECTOR3 delta);

	//부모가 있는 경우 로컬 기준으로 이동 시킨다.
	void MovePositionLocal(float dx, float dy, float dz);
	void MovePositionLocal(D3DXVECTOR3 delta);

	//스케일 셋팅 
	void SetScale(float x, float y, float z);
	void SetScale(D3DXVECTOR3 scale);

	//스케일링
	void Scaling(float dx, float dy, float dz);
	void Scaling(D3DXVECTOR3 deltaScale);

	//월드 기준으로 회전 
	void RotateWorld(float angleX, float angleY, float angleZ);
	void RotateWorld(D3DXVECTOR3 angle);

	//자신의 축기준으로 회전
	void RotateSelf(float angleX, float angleY, float angleZ);
	void RotateSelf(D3DXVECTOR3 angle);

	//부모가 있는 경우 부모 로컬의 축기준으로 회전
	void RotateLocal(float angleX, float angleY, float angleZ);
	void RotateLocal(D3DXVECTOR3 angle);


	//특정 방향을 바라보게 회전해라.
	void LookDirection(const D3DXVECTOR3& dir, const D3DXVECTOR3& up = D3DXVECTOR3(0, 1, 0));

	//특정 방향을 바라보는데 angle 각만큼만 회전 해라
	void LookDirection(const D3DXVECTOR3& dir, float angle);

	//특정위치를 바라보게 회전해라.
	void LookPosition(const D3DXVECTOR3& pos, const D3DXVECTOR3& up = D3DXVECTOR3(0, 1, 0));

	//특정위치를  바라보는데 angle 각만큼만 회전 해라
	void LookPosition(const D3DXVECTOR3& dir, float angle);

	// 자신의 회전 값을 from 과 to 사이의 회전량만큼 회전보간(구면보간) 하여 적용
	void RotateSlerp(const cTransform& from, const cTransform& to, float t);

	// 자신의 위치 값을 from 과 to 사이의 위치만큼 선형보간 하여 적용
	void PositionLerp(const cTransform& from, const cTransform& to, float t);

	// 자신의 모든 정보를 from 과 to 사이의 정보만큼 보간 하여 적용
	void Interpolate(const cTransform& from, const cTransform& to, float t);

	//사원수를 이용한 특정 회전값으로 회전량을 가져라....
	void SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ);
	void SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ);

	// 회전 행렬을 넣어주면 그 회전 행렬대로 회전한다.
	void SetRotateWorld(const D3DXMATRIXA16& matWorldRotate);
	void SetRotateLocal(const D3DXMATRIXA16& matWorldRotate);

	// 회전 사원수를 넣어주면 그 회전값 대로 회전한다.
	void SetRotateWorld(const D3DXQUATERNION& matWorldRotate);
	void SetRotateLocal(const D3DXQUATERNION& matWorldRotate);

	// 월드 행렬을 넣어주면 그 대로 값을 가지게 된다.
	void SetWorldMatrix(const D3DXMATRIXA16& matWorld);

	//
	// Update 들
	//

	//Transform 을 업데이트 한다 ( Trasform 의 정보가 갱신되었을대 사용된다 )
	void UpdateTransform();

	//Device 에 자신의 Tansform 을 월드로 셋팅
	void SetDeviceWorld(LPDIRECT3DDEVICE9 pDevice);

	//Device 에 자신의 Tansform 을 뷰로 셋팅
	void SetDeviceView(LPDIRECT3DDEVICE9 pDevice);




	//
	// Get 들..................
	//
	//최종 행렬을 얻는다.
	D3DXMATRIXA16 GetFinalMatrix() const;
	D3DXMATRIXA16 GetWorldRotateMatrix() const;
	D3DXQUATERNION GetWorldRotateQuaternion() const;



	//위치 값을 얻는다.
	D3DXVECTOR3 GetWorldPosition() const;
	D3DXVECTOR3 GetLocalPosition() const;

	//축을 얻는다. ( 월드 기준 )
	void GetScaledAxies(D3DXVECTOR3* pVecArr) const;
	void GetUnitAxies(D3DXVECTOR3* pVecArr) const;
	D3DXVECTOR3 GetScaledAxis(int axisNum) const;
	D3DXVECTOR3 GetUnitAxis(int axisNum) const;
	D3DXVECTOR3 GetForward(bool bNormalize = true) const;
	D3DXVECTOR3 GetUp(bool bNormalize = true) const;
	D3DXVECTOR3 GetRight(bool bNormalize = true) const;

	D3DXVECTOR3 GetScale() const { return this->scale; }

	//디폴트 컨트롤 을해준다.
	void DefaultControl(float timeDelta);
	void DefaultControl2(float timeDelta);
	void DefaultControl3(float timeDelta);
	void PlayerControll(float timeDelta);

	//Transform 에 대한 기즈모를 그린다.
	void RenderGimozo(bool applyScale = false) const;


	//
	// Shake 관련..................
	//

	//쉐이크 명령이 있으면 Update 를 해주어야 한다.
	void ShakeUpdate(float timeDelta);

	//위치에 대한 흔들림을 준다.
	void ShakePos(float shakePower, float shakeRelase);

	//회전에 대한 흔들림을 준다.
	void ShakeRot(float shakePower, float shakeRelase);

	//흔들림 축 설정
	void SetShakePosFlag(int flag);
	void SetShakeRotFlag(int flag);
	cTransform* GetChild()
	{
		return pFirstChild;
	}



};

