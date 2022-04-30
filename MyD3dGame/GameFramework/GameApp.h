#ifndef GAMEAPP_H
#define GAMEAPP_H
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
	GameObject* m_box = nullptr;
	Mesh m_boxMesh = Geometry::createBoxMesh();
};


#endif