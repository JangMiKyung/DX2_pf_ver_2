#pragma once
class cScene
{
private:


public:
	cScene( void );
	virtual ~cScene( void );

	//4�ѻ��Լ��� �߻��Լ���......
	virtual HRESULT Init() = 0;
	virtual void Release() = 0;
	virtual void Update( float timeDelta ) = 0;
	virtual void Render() = 0;

private:
};

