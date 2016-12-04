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
	bool		bActive; //Ȱ��ȭ ����

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

	//������ �� �޽��� �����Ѵ�.
	void SetMesh(cXMesh*	pMesh){
		this->pMesh = pMesh;
		this->ComputeBoundBox();
	}

	//Ȱ��ȭ ����
	void SetActive(bool bActive);

	//Ȱ��ȭ ���θ� ��´�.
	bool IsActive() {
		return this->bActive;
	}


	//���õ� �޽��� ���� �ٿ��ڽ��� ���� �Ѵ�.
	void ComputeBoundBox();

protected:

	//override �ؼ� ���ÿ�....
	virtual void BaseObjectEnable() {}							//BaseObject �� Ȱ��ȭ �ɶ� ����
	virtual void BaseObjectDisable(){}							//BaseObject �� ��Ȱ��ȭ �ɶ� ����
	virtual void BaseObjectUpdate(float timeDelta) {}			//BaseObject �� Update �� ����....
	virtual void BaseObjectNoActiveUpdate(float timeDelte){}	//BaseObject �� ��Ȱ��ȭ�� ������Ʈ ����....

	virtual void BaseObjectRender();							//BaseObject �� �׸��� ����
};

