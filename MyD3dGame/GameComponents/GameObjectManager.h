#pragma once
#include "GameObject.h"
class GameObjectManager
{
private:
	GameObjectManager();
public:
	void init(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext);
	void onUpdateTime(float dt);
	void draw(bool bShowMesh);
	void setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
	~GameObjectManager();

public:
	static GameObjectManager& getInstance()
	{
		return gameObjectManager;
	}
	std::vector<GameObject*> getGoVector() { return m_goVector; };
private:
	std::vector<GameObject*> m_goVector;
	float m_aspectRatio;
	ComPtr<ID3D11Device> m_pd3dDevice;
	ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;

private:
	Mesh m_boxMesh = Geometry::createBoxMesh(1.0f, 1.0f, 1.0f);
	Mesh m_aroundMesh = Geometry::createPlaneMesh(20.0f, 8.0f, 5.0f, 1.5f);
	Mesh m_groundMesh = Geometry::createPlaneMesh(20.0f, 20.0f, 5.0f, 5.5f);
	Mesh m_waterMesh = Geometry::createPlaneMesh(20.0f, 20.0f, 10.0f, 10.0f);
	Mesh m_wireFenceMesh = Geometry::createBoxMesh();
	Mesh m_mirrorMesh = Geometry::createPlaneMesh(8.0f, 8.0f, 1.0f, 1.0f);

	GameObject* m_box = nullptr;
	GameObject* m_around[5] = { nullptr };//前后左右墙
	GameObject* m_ground = nullptr;
	GameObject* m_water = nullptr;
	GameObject* m_wireFence = nullptr;
	GameObject* m_mirror = nullptr;

private:
	static GameObjectManager gameObjectManager;

};