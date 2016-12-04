#pragma once
class cScene
{
protected:
	cCamera							mainCamera;
	LPDIRECT3DCUBETEXTURE9			evironmentTexture;			//환경 Texture
	LPD3DXMESH						evironmemtSphereMesh;		//환경 구
	LPD3DXEFFECT					evironmentEffect;			//환경 Effect

public:
	cScene( void );
	virtual ~cScene( void );

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render();

	void SetEnvironment(std::string cubeFilePath);

private:

	//씬에서 호출될 함수들을 추상함수로...
	virtual HRESULT Scene_Init() = 0;
	virtual void Scene_Release() = 0;
	virtual void Scene_Update(float timDelta) = 0;

	virtual void Scene_Render0(){}
	virtual void Scene_Render1() = 0;
	virtual void Scene_Render2(){}

	virtual void Scene_RenderSprite(){}

	//환경구 랜더
	void RenderEnvironment();
};

