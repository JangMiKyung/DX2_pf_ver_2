#pragma once


#define RESET_POSITION	1
#define RESET_ROTATION	2
#define RESET_SCALE		4

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

//ȸ�� �����
#define SHAKE_X 1
#define SHAKE_Y 2
#define SHAKE_Z 4

__declspec(align(16))
class cTransform
{
protected:

	//�������� Union ���� ���� axis �迭�ε� ���� �����ϰ�, right, up, forward �� �� ���ٰ����ϴ�.
	// ( �θ� ������ �θ𿡴��� �������� ���ù��� ������ ������� )
	union {

		struct{
			D3DXVECTOR3		axis[3];
		};

		struct{
			//�ٶ󺸴� ���� �� �Ʒ��� 3���� �ݵ�� ����ȭ �Ǿ��־�� �Ѵ�.
			//�׻�����ȭ �Ǿ��ִ�.
			D3DXVECTOR3		right;
			D3DXVECTOR3		up;
			D3DXVECTOR3		forward;
		};
	};


	//��ġ��  ( �θ� ������ �θ𿡴��� �������� ������ġ ������ ���� ��ġ )
	D3DXVECTOR3		position;

	//������ �� 
	D3DXVECTOR3		scale;

	//�������	
	D3DXMATRIXA16	matLocal;				//�θ� ���������� �������� ��� ( ���� �θ� ���ٸ� matFinal �̶� ���� )

	//���� ���		( �θ� �ֵ���� ������忡 ��������� )
	D3DXMATRIXA16	matFinal;


	//�θ� ���� ( null �̸� �θ� ���¾� )
	cTransform*		pParent;

	//�ڽ� ���� ( null �̸� �ڽ��� ���¾� )
	cTransform*		pFirstChild;

	//���� ���� ( null �̸� �ܾƵ� )
	cTransform*		pNextSibling;

	//transform ��ȭ�� �ڵ����� ������Ʈ�� �ɲ���? ( ���� �̰� false �̸� ����ڰ� ���� UpdateTransform ��ȭ�� ȣ�� ���־�� �Ѵ� )
	bool			bAutoUpdate;


	//
	// Shake ����
	//
	float ShakePosPower;			//���� ��ġ�� ���� ��鸲 �Ŀ�
	float ShakeRotPower;			//���� ȸ���� ���� ��鸲 �Ŀ�

	float ShakePosRelease;			//��ġ �Ŀ� �ʴ� ������
	float ShakeRotRelease;			//ȸ�� �Ŀ� �ʴ� ������

	int ShakePosFlag;				//��ġ ����� �÷���
	int ShakeRotFlag;				//ȸ�� ����� �÷���



public:
	cTransform(void);
	~cTransform(void);

	//���� ������Ʈ ���� ����
	void SetAutoUpdate(bool bAuto)
	{
		this->bAutoUpdate = bAuto;
	}

	//Transform ������ �ʱ�ȭ
	void Reset(int resetFlag = -1);

	//Ư�� Child �� ���ڽ����� ���δ�.
	void AddChild(cTransform* pChild);

	//Ư�� Transform �� �ٴ´�.
	void AttachTo(cTransform* pParent);

	//�θ�� �ȳ�
	void ReleaseParent();

	//��ġ�� ���� ��ǥ��� �����Ѵ�. 
	void SetWorldPosition(float x, float y, float z);
	void SetWorldPosition(D3DXVECTOR3 pos);

	//��ġ�� ���� ��ǥ��� �����Ѵ�.  ( �θ� �ִ� ��� ������ �������� ��ġ )
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(D3DXVECTOR3 pos);

	//�ڽ��� ��������� �̵� ��Ų��.
	void MovePositionSelf(float dx, float dy, float dz);
	void MovePositionSelf(D3DXVECTOR3 delta);

	//���� �������� �̵� ��Ų��.
	void MovePositionWorld(float dx, float dy, float dz);
	void MovePositionWorld(D3DXVECTOR3 delta);

	//�θ� �ִ� ��� ���� �������� �̵� ��Ų��.
	void MovePositionLocal(float dx, float dy, float dz);
	void MovePositionLocal(D3DXVECTOR3 delta);

	//������ ���� 
	void SetScale(float x, float y, float z);
	void SetScale(D3DXVECTOR3 scale);

	//�����ϸ�
	void Scaling(float dx, float dy, float dz);
	void Scaling(D3DXVECTOR3 deltaScale);

	//���� �������� ȸ�� 
	void RotateWorld(float angleX, float angleY, float angleZ);
	void RotateWorld(D3DXVECTOR3 angle);

	//�ڽ��� ��������� ȸ��
	void RotateSelf(float angleX, float angleY, float angleZ);
	void RotateSelf(D3DXVECTOR3 angle);

	//�θ� �ִ� ��� �θ� ������ ��������� ȸ��
	void RotateLocal(float angleX, float angleY, float angleZ);
	void RotateLocal(D3DXVECTOR3 angle);


	//Ư�� ������ �ٶ󺸰� ȸ���ض�.
	void LookDirection(const D3DXVECTOR3& dir, const D3DXVECTOR3& up = D3DXVECTOR3(0, 1, 0));

	//Ư�� ������ �ٶ󺸴µ� angle ����ŭ�� ȸ�� �ض�
	void LookDirection(const D3DXVECTOR3& dir, float angle);

	//Ư����ġ�� �ٶ󺸰� ȸ���ض�.
	void LookPosition(const D3DXVECTOR3& pos, const D3DXVECTOR3& up = D3DXVECTOR3(0, 1, 0));

	//Ư����ġ��  �ٶ󺸴µ� angle ����ŭ�� ȸ�� �ض�
	void LookPosition(const D3DXVECTOR3& dir, float angle);

	// �ڽ��� ȸ�� ���� from �� to ������ ȸ������ŭ ȸ������(���麸��) �Ͽ� ����
	void RotateSlerp(const cTransform& from, const cTransform& to, float t);

	// �ڽ��� ��ġ ���� from �� to ������ ��ġ��ŭ �������� �Ͽ� ����
	void PositionLerp(const cTransform& from, const cTransform& to, float t);

	// �ڽ��� ��� ������ from �� to ������ ������ŭ ���� �Ͽ� ����
	void Interpolate(const cTransform& from, const cTransform& to, float t);

	//������� �̿��� Ư�� ȸ�������� ȸ������ ������....
	void SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ);
	void SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ);

	// ȸ�� ����� �־��ָ� �� ȸ�� ��Ĵ�� ȸ���Ѵ�.
	void SetRotateWorld(const D3DXMATRIXA16& matWorldRotate);
	void SetRotateLocal(const D3DXMATRIXA16& matWorldRotate);

	// ȸ�� ������� �־��ָ� �� ȸ���� ��� ȸ���Ѵ�.
	void SetRotateWorld(const D3DXQUATERNION& matWorldRotate);
	void SetRotateLocal(const D3DXQUATERNION& matWorldRotate);

	// ���� ����� �־��ָ� �� ��� ���� ������ �ȴ�.
	void SetWorldMatrix(const D3DXMATRIXA16& matWorld);

	//
	// Update ��
	//

	//Transform �� ������Ʈ �Ѵ� ( Trasform �� ������ ���ŵǾ����� ���ȴ� )
	void UpdateTransform();

	//Device �� �ڽ��� Tansform �� ����� ����
	void SetDeviceWorld(LPDIRECT3DDEVICE9 pDevice);

	//Device �� �ڽ��� Tansform �� ��� ����
	void SetDeviceView(LPDIRECT3DDEVICE9 pDevice);




	//
	// Get ��..................
	//
	//���� ����� ��´�.
	D3DXMATRIXA16 GetFinalMatrix() const;
	D3DXMATRIXA16 GetWorldRotateMatrix() const;
	D3DXQUATERNION GetWorldRotateQuaternion() const;



	//��ġ ���� ��´�.
	D3DXVECTOR3 GetWorldPosition() const;
	D3DXVECTOR3 GetLocalPosition() const;

	//���� ��´�. ( ���� ���� )
	void GetScaledAxies(D3DXVECTOR3* pVecArr) const;
	void GetUnitAxies(D3DXVECTOR3* pVecArr) const;
	D3DXVECTOR3 GetScaledAxis(int axisNum) const;
	D3DXVECTOR3 GetUnitAxis(int axisNum) const;
	D3DXVECTOR3 GetForward(bool bNormalize = true) const;
	D3DXVECTOR3 GetUp(bool bNormalize = true) const;
	D3DXVECTOR3 GetRight(bool bNormalize = true) const;

	D3DXVECTOR3 GetScale() const { return this->scale; }

	//����Ʈ ��Ʈ�� �����ش�.
	void DefaultControl(float timeDelta);
	void DefaultControl2(float timeDelta);
	void DefaultControl3(float timeDelta);
	void PlayerControll(float timeDelta);

	//Transform �� ���� ����� �׸���.
	void RenderGimozo(bool applyScale = false) const;


	//
	// Shake ����..................
	//

	//����ũ ����� ������ Update �� ���־�� �Ѵ�.
	void ShakeUpdate(float timeDelta);

	//��ġ�� ���� ��鸲�� �ش�.
	void ShakePos(float shakePower, float shakeRelase);

	//ȸ���� ���� ��鸲�� �ش�.
	void ShakeRot(float shakePower, float shakeRelase);

	//��鸲 �� ����
	void SetShakePosFlag(int flag);
	void SetShakeRotFlag(int flag);
	cTransform* GetChild()
	{
		return pFirstChild;
	}



};

