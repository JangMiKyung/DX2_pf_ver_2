#pragma once

class cTransform;

class State
{
protected:
	// �̵� Ȱ��ȭ ��ų������
	bool isActive;
	bool isRotateActive;
	bool isFinish;
	// �̵��� �� ���� ���� 
	float MoveLerfPt;
	// ȸ���Ҷ� ���� ���� 
	float RotateLerfPt;
	string stateName;
	float m_Speed; // (������ �̵� ���ǵ� )
	float m_RotateSpeed; // (ȸ�� �󸶳� ���� �Ұ��ΰ�
	cTransform* m_Trans;  // ������ Ʈ������ 
	
	
public:
	State() {};
	State(float speed, float RotateSpeed, cTransform* Trans, string name ) : m_Speed(speed), m_RotateSpeed(RotateSpeed), m_Trans(Trans), stateName(name)
	{   
		isRotateActive = false;
		isActive = false;
		isFinish = false;
		//���� ���� ó���� 0 ~
		MoveLerfPt = 0;
		RotateLerfPt = 0;
	}
	~State() {};
	//virtual void Update(float timeUpdate);

	// ���� �̵� ��ų�� ������ ����� �̵���, 
	//�������� ������ ��������.
	virtual void Move(float timeUpdate, cTransform Sour, cTransform Dest);
	//�÷��̾� �̵��� �Լ�,
	virtual void Move(float timeUpdate);

	// ���� ȸ���Ҷ� Angle or Vector ������ŭ ������. �������� �̿��� õõ�� ������ �Լ���.
	virtual void Rotate(float timeUpdate, float Angle);

	//Set �Լ���
	virtual void setActive(bool TorF) { isActive = TorF; }
	virtual void setRotateActive(bool TorF) { isRotateActive = TorF; }
	virtual void setMoveSpeed(float speed){ m_Speed = speed;}
	virtual void setRotateSpeed(float speed) { m_RotateSpeed = speed; }
	virtual void setTransform(cTransform* Trans) {	m_Trans = Trans; }

	//Get �Լ���!
	virtual bool GetisActive() { return isActive; }
	virtual string getAniname() { return stateName;  }

};





