#pragma once

#include "d3dApp.h"
#include "../GameComponents/GameObject.h"

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
private:

	
	Mesh m_boxMesh = Geometry::createBoxMesh();
	Mesh m_aroundMesh = Geometry::createPlane(20.0f, 8.0f, 5.0f, 1.5f);
	Mesh m_groundMesh = Geometry::createPlane(20.0f, 20.0f, 5.0f, 5.5f);

	GameObject* m_box = nullptr;
	GameObject* m_around[4] = { nullptr };//前后左右墙
	GameObject* m_ground = nullptr;

};


