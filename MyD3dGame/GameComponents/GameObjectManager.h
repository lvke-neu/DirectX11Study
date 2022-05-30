#pragma once
#include "GameObject.h"
class GameObjectManager
{

public:
	void init(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext);
	void onUpdateTime(float dt);
	void draw();
	void setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
	~GameObjectManager();

public:
	static GameObjectManager& getInstance()
	{
		return gameObjectManager;
	}

private:
	std::vector<GameObject*> m_goVector;
	float m_aspectRatio;
	ComPtr<ID3D11Device> m_pd3dDevice;
	ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;

private:
	Mesh m_boxMesh = Geometry::createBoxMesh();
	Mesh m_aroundMesh = Geometry::createPlane(20.0f, 8.0f, 5.0f, 1.5f);
	Mesh m_groundMesh = Geometry::createPlane(20.0f, 20.0f, 5.0f, 5.5f);

	GameObject* m_box = nullptr;
	GameObject* m_around[4] = { nullptr };//前后左右墙
	GameObject* m_ground = nullptr;

private:
	static GameObjectManager gameObjectManager;

};