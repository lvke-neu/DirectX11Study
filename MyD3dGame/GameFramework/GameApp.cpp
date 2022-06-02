#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
#include "../GameComponents/Camera.h"
#include "RenderStates.h"
GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

GameApp::~GameApp()
{

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

	GameObjectManager::getInstance().setAspectRatio(AspectRatio());
}

void GameApp::UpdateScene(float dt)
{

#ifdef USE_IMGUI

	cameraController(dt);
	wireFenceController(dt);
	boxController(dt);

	ImGui::Begin("Camera");
	ImGui::Text("Camera Position\n%.2f %.2f %.2f", Camera::getInstance().getPosition().x, Camera::getInstance().getPosition().y, Camera::getInstance().getPosition().z);
	ImGui::Text("Rasterization\n");
	ImGui::Checkbox("Show Mesh", &m_bShowMesh);
	ImGui::End();

	ImGui::Render();
#endif

	//不管有没有变换都更新一遍
	GameObjectManager::getInstance().onUpdateTime(dt);
}

void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);
	static float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	
	GameObjectManager::getInstance().draw(m_bShowMesh);

#ifdef USE_IMGUI
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	HR(m_pSwapChain->Present(0, 0));
}

void GameApp::InitResource()
{	/*
	1、初始化相机
	2、初始化GameObject
	*/
	Camera::getInstance().setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 2.0f, -5.0f)));
	GameObjectManager::getInstance().init(m_pd3dDevice, m_pd3dImmediateContext);

	//初始化各种渲染状态状态
	RenderStates::InitAll(m_pd3dDevice.Get());
}

void GameApp::cameraController(float dt)
{
	ImGuiIO& io = ImGui::GetIO();

	//右键按下调整相机姿态
	static XMFLOAT3 caRot = Camera::getInstance().getRotation();
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		caRot.y += io.MouseDelta.x * dt * 10;
		caRot.x += io.MouseDelta.y * dt * 10;
	}
	Camera::getInstance().setRotation(caRot);

	//滑动滑轮使得相机往下降或者往上升
	Camera::getInstance().moveForward(io.MouseWheel);


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

}

void GameApp::wireFenceController(float dt)
{
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::IsKeyDown(0x26))
		GameObjectManager::getInstance().getGoVector()[1]->moveForward(dt * 10);
	if (ImGui::IsKeyDown(0x28))
		GameObjectManager::getInstance().getGoVector()[1]->moveForward(-dt * 10);
	if (ImGui::IsKeyDown(0x25))
		GameObjectManager::getInstance().getGoVector()[1]->moveRight(-dt * 10);
	if (ImGui::IsKeyDown(0x27))
		GameObjectManager::getInstance().getGoVector()[1]->moveRight(dt * 10);

	XMFLOAT3 adjustedPos;
	XMStoreFloat3(&adjustedPos, XMVectorClamp(XMLoadFloat3(&GameObjectManager::getInstance().getGoVector()[1]->getPosition()), XMVectorSet(-8.9f, 0.0f, -8.9f, 0.0f), XMVectorReplicate(8.9f)));
	GameObjectManager::getInstance().getGoVector()[1]->setPosition(adjustedPos);

}
void GameApp::boxController(float dt)
{
	static float rotx = 0;
	static float roty = 0;

	rotx += dt;
	roty += dt;

	XMFLOAT3 boxRot = GameObjectManager::getInstance().getGoVector()[0]->getRotation();
	boxRot.x = rotx;
	boxRot.y = roty;

	GameObjectManager::getInstance().getGoVector()[0]->setRotation(boxRot);

}