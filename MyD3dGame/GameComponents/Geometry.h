
//先只支持 pos+normal+tex类型的点
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

//Mesh数据包括 vertex与index
struct Mesh   
{
	std::vector<VertexPosNormalTex> vertexbuffer;
	std::vector<unsigned long> indexbuffer;
};

class Geometry
{
public:
	static Mesh createBoxMesh(float width = 2.0f, float height = 2.0f, float depth = 2.0f); //创建长宽高为1的立方体
	static Mesh createPlaneMesh(float width, float depth, float texU, float texV);//创建面
};