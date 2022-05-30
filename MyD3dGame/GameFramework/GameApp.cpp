#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"


GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

GameApp::~GameApp()
{
	if (m_box)
		delete m_box;

	for (int i = 0; i < 4; ++i)
		if (m_around[i])
			delete m_around[i];

	if (m_ground)
		delete m_ground;
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	InitResource();

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{

	/*
	让盒子动起来
	*/
	static float rotx = 0;
	static float roty = 0;

	rotx += dt;
	roty += dt;

	XMFLOAT3 boxRot = m_box->getRotation();
	boxRot.x = rotx;
	boxRot.y = roty;

	m_box->setRotation(boxRot);

#ifdef USE_IMGUI
	ImGuiIO& io = ImGui::GetIO();

	static XMFLOAT3 caRot = Camera::getInstance().getRotation();
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		caRot.y += io.MouseDelta.x * dt * 10;
		caRot.x += io.MouseDelta.y * dt * 10;
	}
	Camera::getInstance().setRotation(caRot);
	
	if (ImGui::IsKeyDown('W'))
		Camera::getInstance().moveForward(dt * 10);
	if (ImGui::IsKeyDown('S'))
		Camera::getInstance().moveForward(-dt * 10);
	if (ImGui::IsKeyDown('A'))
		Camera::getInstance().moveRight(-dt * 10);
	if (ImGui::IsKeyDown('D'))
		Camera::getInstance().moveRight(dt * 10);

	XMFLOAT3 adjustedPos;
	XMStoreFloat3(&adjustedPos, XMVectorClamp(XMLoadFloat3(&Camera::getInstance().getPosition()), XMVectorSet(-8.9f, 0.0f, -8.9f, 0.0f), XMVectorReplicate(8.9f)));
	Camera::getInstance().setPosition(adjustedPos);

	
	ImGui::Begin("Camera");
	ImGui::Text("Camera Position\n%.2f %.2f %.2f", Camera::getInstance().getPosition().x, Camera::getInstance().getPosition().y, Camera::getInstance().getPosition().z);
	ImGui::Text("Rasterization\n");
	ImGui::Checkbox("Show Mesh", &m_bShowMesh);
	ImGui::End();

	ImGui::Render();
#endif


	gameObjectUpdate();
	Camera::getInstance().startRasterization(m_bShowMesh, m_pd3dDevice, m_pd3dImmediateContext);
}

void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);
	static float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	/*盒子*/
	m_box->draw(m_boxMesh.indexbuffer.size());
	/*四面墙*/
	for (int i = 0; i < 4; ++i)
	{
		m_around[i]->draw(m_aroundMesh.indexbuffer.size());
	}
	/*地板*/
	m_ground->draw(m_groundMesh.indexbuffer.size());


#ifdef USE_IMGUI
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	HR(m_pSwapChain->Present(0, 0));
}

void GameApp::InitResource()
{	/*
	1、初始化相机
	2、初始化光源
	3、初始化GameObject
	*/
	Camera::getInstance().setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -5.0f)));
	
	gameObjectInit();
}


void GameApp::gameObjectInit()
{
	/*盒子*/
	if (m_box)
		delete m_box;
	m_box = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_box->init(m_boxMesh, L"Texture\\WoodCrate.dds",
		Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)), AspectRatio());

	/*四面墙*/
	for (int i = 0; i < 4; ++i)
		if (m_around[i])
			delete m_around[i];
	for (int i = 0; i < 4; ++i)
	{
		m_around[i] = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
		m_around[i]->init(m_aroundMesh, L"Texture\\brick.dds",
			Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-XM_PIDIV2, XM_PIDIV2 * i, 0.0f), XMFLOAT3(i % 2 ? -10.0f * (i - 2) : 0.0f, 3.0f, i % 2 == 0 ? -10.0f * (i - 1) : 0.0f)),
			AspectRatio());
	}

	/*地板*/
	if (m_ground)
		delete m_ground;
	m_ground = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_ground->init(m_groundMesh, L"Texture\\checkboard.dds",
		Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)),
		AspectRatio());
}

void GameApp::gameObjectUpdate()
{
	/*盒子*/
	m_box->updateWorldViewProjMatrix(AspectRatio());
	m_box->updateLight(DLight);
	/*四面墙*/
	for (int i = 0; i < 4; ++i)
	{
		m_around[i]->updateWorldViewProjMatrix(AspectRatio());
		m_around[i]->updateLight(DLight);
	}

	/*地板*/
	m_ground->updateWorldViewProjMatrix(AspectRatio());
	m_ground->updateLight(DLight);
}