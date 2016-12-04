#pragma once
class cScene
{
protected:
	cCamera							mainCamera;
	LPDIRECT3DCUBETEXTURE9			evironmentTexture;			//ȯ�� Texture
	LPD3DXMESH						evironmemtSphereMesh;		//ȯ�� ��
	LPD3DXEFFECT					evironmentEffect;			//ȯ�� Effect

public:
	cScene( void );
	virtual ~cScene( void );

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render();

	void SetEnvironment(std::string cubeFilePath);

private:

	//������ ȣ��� �Լ����� �߻��Լ���...
	virtual HRESULT Scene_Init() = 0;
	virtual void Scene_Release() = 0;
	virtual void Scene_Update(float timDelta) = 0;

	virtual void Scene_Render0(){}
	virtual void Scene_Render1() = 0;
	virtual void Scene_Render2(){}

	virtual void Scene_RenderSprite(){}

	//ȯ�汸 ����
	void RenderEnvironment();
};

