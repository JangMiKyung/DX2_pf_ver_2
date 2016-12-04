#pragma once

class cXMesh;
class cTransform;

class cBaseObject
{
public:
	cTransform*	pTransform;
	cXMesh*		pMesh;
	cBoundBox	BoundBox;

protected:
	bool		bActive; //활성화 여부

public:
	cBaseObject();
	~cBaseObject();

	void Update(float timeDelta){
		if (this->bActive)
			this->BaseObjectUpdate(timeDelta);
		else
			this->BaseObjectNoActiveUpdate(timeDelta);
	}

	void Render(){
		if (this->bActive)
			this->BaseObjectRender();
	}

	//랜더링 될 메쉬를 셋팅한다.
	void SetMesh(cXMesh*	pMesh){
		this->pMesh = pMesh;
		this->ComputeBoundBox();
	}

	//활성화 셋팅
	void SetActive(bool bActive);

	//활성화 여부를 얻는다.
	bool IsActive() {
		return this->bActive;
	}


	//셋팅된 메쉬에 따라 바운드박스를 재계산 한다.
	void ComputeBoundBox();

protected:

	//override 해서 쓰시오....
	virtual void BaseObjectEnable() {}							//BaseObject 가 활성화 될때 실행
	virtual void BaseObjectDisable(){}							//BaseObject 가 비활성화 될때 실행
	virtual void BaseObjectUpdate(float timeDelta) {}			//BaseObject 가 Update 때 실행....
	virtual void BaseObjectNoActiveUpdate(float timeDelte){}	//BaseObject 가 비활성화시 업데이트 실행....

	virtual void BaseObjectRender();							//BaseObject 를 그릴때 실행
};

