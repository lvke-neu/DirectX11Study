#include "GameObjectManager.h"
#include "../GameFramework/RenderStates.h"
GameObjectManager GameObjectManager::gameObjectManager;


void GameObjectManager::init(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext)
{
	m_pd3dDevice = pd3dDevice;
	m_pd3dImmediateContext = pd3dImmediateContext;

	Material material; 
	material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	/*盒子*/
	m_box = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_box->setMesh(Geometry::createBoxMesh(1.0f, 1.0f, 1.0f));
	m_box->setShader();
	m_box->setTexture(L"Texture\\WoodCrate.dds");
	m_box->setMaterial(material);
	m_box->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 2.0f, 0.0f)));
	m_goVector.push_back(m_box);


	/*铁丝围栏*/
	m_wireFence = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_wireFence->setMesh(Geometry::createBoxMesh(1.0f, 1.0f, 1.0f));
	m_wireFence->setShader();
	m_wireFence->setTexture(L"Texture\\WireFence.dds");
	m_wireFence->setMaterial(material);
	m_goVector.push_back(m_wireFence);


	/*地板*/
	m_ground = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_ground->setMesh(Geometry::createPlaneMesh(20.0f, 20.0f, 5.0f, 5.0f));
	m_ground->setShader();
	m_ground->setTexture(L"Texture\\floor.dds");
	m_ground->setMaterial(material);
	m_ground->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)));
	m_goVector.push_back(m_ground);


	/*四面墙*/
	for (int i = 0; i < 5; ++i)
	{
		m_around[i] = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
		m_around[i]->setShader();
		m_around[i]->setTexture(L"Texture\\brick.dds");
		m_around[i]->setMaterial(material);
		m_goVector.push_back(m_around[i]);
	}

	m_around[0]->setMesh(Geometry::createPlaneMesh(6.0f, 8.0f, 1.5f, 2.0f));
	m_around[1]->setMesh(Geometry::createPlaneMesh(6.0f, 8.0f, 1.5f, 2.0f));
	m_around[2]->setMesh(Geometry::createPlaneMesh(20.0f, 8.0f, 5.0f, 2.0f));
	m_around[3]->setMesh(Geometry::createPlaneMesh(20.0f, 8.0f, 5.0f, 2.0f));
	m_around[4]->setMesh(Geometry::createPlaneMesh(20.0f, 8.0f, 5.0f, 2.0f));

	m_around[0]->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-XM_PIDIV2, 0.0f, 0.0f), XMFLOAT3(-7.0f, 3.0f, 10.0f)));
	m_around[1]->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-XM_PIDIV2, 0.0f, 0.0f), XMFLOAT3(7.0f, 3.0f, 10.0f)));
	m_around[2]->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-XM_PIDIV2, XM_PIDIV2, 0.0f), XMFLOAT3(10.0f, 3.0f, 0.0f)));
	m_around[3]->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-XM_PIDIV2, XM_PI, 0.0f), XMFLOAT3(0.0f, 3.0f, -10.0f)));
	m_around[4]->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-XM_PIDIV2, -XM_PIDIV2, 0.0f), XMFLOAT3(-10.0f, 3.0f, 0.0f)));

	material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	material.specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);

	/*水*/
	m_water = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_water->setMesh(Geometry::createPlaneMesh(20.0f, 20.0f, 10.0f, 10.0f));
	m_water->setShader();
	m_water->setTexture(L"Texture\\water.dds");
	m_water->setMaterial(material);
	m_water->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)));
	m_goVector.push_back(m_water);



	material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	material.specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);
	/*镜子*/
	m_mirror = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_mirror->setMesh(Geometry::createPlaneMesh(8.0f, 8.0f, 1.0f, 1.0f));
	m_mirror->setShader();
	m_mirror->setTexture(L"Texture\\ice.dds");
	m_mirror->setMaterial(material);
	m_mirror->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-XM_PIDIV2, 0.0f, 0.0f), XMFLOAT3(0.0f, 3.0f, 10.0f)));
	m_goVector.push_back(m_mirror);
}

void GameObjectManager::onUpdateTime(float dt)
{
	for (auto go : m_goVector)
	{
		if (go)
		{
			go->updateWorldViewProjMatrix(m_aspectRatio, 0);
		}
	}
}

void GameObjectManager::draw(bool bShowMesh)
{
	////绘制不透明对象
	//m_pd3dImmediateContext->RSSetState(nullptr);
	//m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	//if (bShowMesh)
	//	m_pd3dImmediateContext->RSSetState(RenderStates::RSWireframe.Get());


	//m_mirror->draw();
	//m_box->draw();
	//for (auto around : m_around)
	//{
	//	around->draw();
	//}
	//m_ground->draw();


	////绘制透明对象
	//m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	//m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSTransparent.Get(), nullptr, 0xFFFFFFFF);
	//if (bShowMesh)
	//	m_pd3dImmediateContext->RSSetState(RenderStates::RSWireframe.Get());


	//m_wireFence->setPosition(XMFLOAT3(2.0f, 1.0f, 0.0f));
	//m_wireFence->updateWorldViewProjMatrix(m_aspectRatio);
	//m_wireFence->draw();

	//m_wireFence->setPosition(XMFLOAT3(-2.0f, 1.0f, 0.0f));
	//m_wireFence->updateWorldViewProjMatrix(m_aspectRatio);
	//m_wireFence->draw();

	//m_water->draw();



	// 1. 给镜面反射区域写入值1到模板缓冲区
	m_pd3dImmediateContext->RSSetState(nullptr);
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSWriteStencil.Get(), 1);
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSNoColorWrite.Get(), nullptr, 0xFFFFFFFF);
	m_mirror->draw();

	// 2. 绘制不透明的反射物体
	m_box->updateWorldViewProjMatrix(m_aspectRatio, 1);
	m_around[2]->updateWorldViewProjMatrix(m_aspectRatio, 1);
	m_around[3]->updateWorldViewProjMatrix(m_aspectRatio, 1);
	m_around[4]->updateWorldViewProjMatrix(m_aspectRatio, 1);
	m_ground->updateWorldViewProjMatrix(m_aspectRatio, 1);
	m_pd3dImmediateContext->RSSetState(RenderStates::RSCullClockWise.Get());
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSDrawWithStencil.Get(), 1);
	m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	m_box->draw();
	m_around[2]->draw();
	m_around[3]->draw();
	m_around[4]->draw();
	m_ground->draw();

	//3. 绘制透明的反射物体
	m_wireFence->updateWorldViewProjMatrix(m_aspectRatio, 1);
	m_water->updateWorldViewProjMatrix(m_aspectRatio, 1);
	m_mirror->updateWorldViewProjMatrix(m_aspectRatio, 1);
	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSDrawWithStencil.Get(), 1);
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSTransparent.Get(), nullptr, 0xFFFFFFFF);
	m_wireFence->draw();
	m_water->draw();
	m_mirror->draw();

	//4. 绘制不透明的正常物体
	m_box->updateWorldViewProjMatrix(m_aspectRatio, 0);
	m_around[2]->updateWorldViewProjMatrix(m_aspectRatio, 0);
	m_around[3]->updateWorldViewProjMatrix(m_aspectRatio, 0);
	m_around[4]->updateWorldViewProjMatrix(m_aspectRatio, 0);
	m_ground->updateWorldViewProjMatrix(m_aspectRatio, 0);
	m_wireFence->updateWorldViewProjMatrix(m_aspectRatio, 0);
	m_water->updateWorldViewProjMatrix(m_aspectRatio, 0);
	m_mirror->updateWorldViewProjMatrix(m_aspectRatio, 0);
	m_pd3dImmediateContext->RSSetState(nullptr);
	m_pd3dImmediateContext->OMSetDepthStencilState(nullptr, 0);
	m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	m_box->draw();
	for (auto& wall : m_around)
		wall->draw();
	m_ground->draw();

	//5. 绘制透明的正常物体
	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->OMSetDepthStencilState(nullptr, 0);
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSTransparent.Get(), nullptr, 0xFFFFFFFF);
	m_wireFence->draw();
	m_water->draw();
}



GameObjectManager::GameObjectManager()
{

}
GameObjectManager::~GameObjectManager()
{
	for (auto go : m_goVector)
	{
		if (go)
		{
			delete go;
		}
	}
}

