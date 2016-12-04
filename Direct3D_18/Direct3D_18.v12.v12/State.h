#pragma once

class cTransform;

class State
{
protected:
	// 이동 활성화 시킬것인지
	bool isActive;
	bool isRotateActive;
	bool isFinish;
	// 이동할 때 보간 비율 
	float MoveLerfPt;
	// 회전할때 보간 비율 
	float RotateLerfPt;
	string stateName;
	float m_Speed; // (유닛의 이동 스피드 )
	float m_RotateSpeed; // (회전 얼마나 빨리 할것인가
	cTransform* m_Trans;  // 유닛의 트랜스폼 
	
	
public:
	State() {};
	State(float speed, float RotateSpeed, cTransform* Trans, string name ) : m_Speed(speed), m_RotateSpeed(RotateSpeed), m_Trans(Trans), stateName(name)
	{   
		isRotateActive = false;
		isActive = false;
		isFinish = false;
		//보간 비율 처음은 0 ~
		MoveLerfPt = 0;
		RotateLerfPt = 0;
	}
	~State() {};
	//virtual void Update(float timeUpdate);

	// 유닛 이동 시킬때 보간을 사용한 이동법, 
	//목적지와 도착지 설정하자.
	virtual void Move(float timeUpdate, cTransform Sour, cTransform Dest);
	//플레이어 이동용 함수,
	virtual void Move(float timeUpdate);

	// 유닛 회전할때 Angle or Vector 지점만큼 돌려라. 보간법을 이용한 천천히 돌리는 함수임.
	virtual void Rotate(float timeUpdate, float Angle);

	//Set 함수들
	virtual void setActive(bool TorF) { isActive = TorF; }
	virtual void setRotateActive(bool TorF) { isRotateActive = TorF; }
	virtual void setMoveSpeed(float speed){ m_Speed = speed;}
	virtual void setRotateSpeed(float speed) { m_RotateSpeed = speed; }
	virtual void setTransform(cTransform* Trans) {	m_Trans = Trans; }

	//Get 함수들!
	virtual bool GetisActive() { return isActive; }
	virtual string getAniname() { return stateName;  }

};





