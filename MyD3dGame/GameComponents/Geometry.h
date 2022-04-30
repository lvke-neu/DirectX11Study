
//��ֻ֧�� pos+normal+tex���͵ĵ�
#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;



struct VertexPosNormalTex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 tex;
};

//Mesh���ݰ��� vertex��index
struct Mesh   
{
	std::vector<VertexPosNormalTex> vertexbuffer;
	std::vector<unsigned long> indexbuffer;
};

class Geometry
{
public:
	static Mesh createBoxMesh(); //���������Ϊ1��������
};