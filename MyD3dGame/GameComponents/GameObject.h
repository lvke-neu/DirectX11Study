#pragma once
#include "Transform.h"
#include "GameObjectResource.h"



template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
class GameObject
{

public:
	GameObject(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext) :
		m_pd3dDevice(pd3dDevice), m_pd3dImmediateContext(pd3dImmediateContext) {}

	void setMesh(const Mesh& mesh);
	void setShader();
	void setTexture( const wchar_t* texturePath);
	void setMaterial(const Material& material);
	void draw();
	void moveForward(float dis);
	void moveRight(float dis);
	

	void setTransform(const Transform& transform){ m_transform = transform; }
	Transform getTransform(){ return m_transform; }
	inline XMFLOAT3 getScale() { return m_transform.getScale(); }
	void setScale(const XMFLOAT3& scale) { m_transform.setScale(scale); }
	inline XMFLOAT3 getRotation() { return m_transform.getRotation(); }
	void setRotation(const XMFLOAT3& rotation) { m_transform.setRotation(rotation); }
	inline XMFLOAT3 getPosition() { return m_transform.getPosition(); }
	void setPosition(const XMFLOAT3& position) { m_transform.setPosition(position); }


private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// 顶点缓冲区
	ComPtr<ID3D11Buffer> m_pIndexBuffer;		// 索引缓冲区


	
	ComPtr<ID3D11VertexShader> m_pVertexShader;	// 顶点着色器
	ComPtr<ID3D11InputLayout> m_pVertexLayout;	// 顶点输入布局
	ComPtr<ID3D11PixelShader> m_pPixelShader;	// 像素着色器

	ComPtr<ID3D11ShaderResourceView> m_pTexture;			    // 纹理


	ComPtr<ID3D11Device> m_pd3dDevice;
	ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;

private:
	Mesh m_mesh;
	const wchar_t* m_texturePath;
	Material m_material;
	Transform m_transform; //变换
};