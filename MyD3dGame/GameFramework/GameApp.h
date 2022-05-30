#pragma once

#include "d3dApp.h"
#include "../GameComponents/GameObjectManager.h"


class GameApp : public D3DApp
{
	
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
	void InitResource();
	void cameraController(float dt);
private:
	bool m_bShowMesh = false;

};


