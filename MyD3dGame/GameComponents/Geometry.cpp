#include "Geometry.h"

Mesh Geometry::createBoxMesh()
{
	Mesh boxMesh;
	boxMesh.vertexbuffer.resize(24);
	// ����(+X��)
	boxMesh.vertexbuffer[0].pos = XMFLOAT3(1, -1, -1);
	boxMesh.vertexbuffer[1].pos = XMFLOAT3(1, 1, -1);
	boxMesh.vertexbuffer[2].pos = XMFLOAT3(1, 1, 1);
	boxMesh.vertexbuffer[3].pos = XMFLOAT3(1, -1, 1);
	// ����(-X��)
	boxMesh.vertexbuffer[4].pos = XMFLOAT3(-1, -1, 1);
	boxMesh.vertexbuffer[5].pos = XMFLOAT3(-1, 1, 1);
	boxMesh.vertexbuffer[6].pos = XMFLOAT3(-1, 1, -1);
	boxMesh.vertexbuffer[7].pos = XMFLOAT3(-1, -1, -1);
	// ����(+Y��)
	boxMesh.vertexbuffer[8].pos = XMFLOAT3(-1, 1, -1);
	boxMesh.vertexbuffer[9].pos = XMFLOAT3(-1, 1, 1);
	boxMesh.vertexbuffer[10].pos = XMFLOAT3(1, 1, 1);
	boxMesh.vertexbuffer[11].pos = XMFLOAT3(1, 1, -1);
	// ����(-Y��)
	boxMesh.vertexbuffer[12].pos = XMFLOAT3(1, -1, -1);
	boxMesh.vertexbuffer[13].pos = XMFLOAT3(1, -1, 1);
	boxMesh.vertexbuffer[14].pos = XMFLOAT3(-1, -1, 1);
	boxMesh.vertexbuffer[15].pos = XMFLOAT3(-1, -1, -1);
	// ����(+Z��)
	boxMesh.vertexbuffer[16].pos = XMFLOAT3(1, -1, 1);
	boxMesh.vertexbuffer[17].pos = XMFLOAT3(1, 1, 1);
	boxMesh.vertexbuffer[18].pos = XMFLOAT3(-1, 1, 1);
	boxMesh.vertexbuffer[19].pos = XMFLOAT3(-1, -1, 1);
	// ����(-Z��)
	boxMesh.vertexbuffer[20].pos = XMFLOAT3(-1, -1, -1);
	boxMesh.vertexbuffer[21].pos = XMFLOAT3(-1, 1, -1);
	boxMesh.vertexbuffer[22].pos = XMFLOAT3(1, 1, -1);
	boxMesh.vertexbuffer[23].pos = XMFLOAT3(1, -1, -1);


	for (int i = 0; i < 4; i++)
	{
		// ����(+X��)
		boxMesh.vertexbuffer[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

		// ����(-X��)
		boxMesh.vertexbuffer[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

		// ����(+Y��)
		boxMesh.vertexbuffer[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

		// ����(-Y��)
		boxMesh.vertexbuffer[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

		// ����(+Z��)
		boxMesh.vertexbuffer[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

		// ����(-Z��)
		boxMesh.vertexbuffer[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	}

	for (int i = 0; i < 6; ++i)
	{
		boxMesh.vertexbuffer[i * 4].tex = XMFLOAT2(0.0f, 1.0f);
		boxMesh.vertexbuffer[i * 4 + 1].tex = XMFLOAT2(0.0f, 0.0f);
		boxMesh.vertexbuffer[i * 4 + 2].tex = XMFLOAT2(1.0f, 0.0f);
		boxMesh.vertexbuffer[i * 4 + 3].tex = XMFLOAT2(1.0f, 1.0f);
	}

	boxMesh.indexbuffer =
	{
		0, 1, 2, 2, 3, 0,		// ����(+X��)
		4, 5, 6, 6, 7, 4,		// ����(-X��)
		8, 9, 10, 10, 11, 8,	// ����(+Y��)
		12, 13, 14, 14, 15, 12,	// ����(-Y��)
		16, 17, 18, 18, 19, 16, // ����(+Z��)
		20, 21, 22, 22, 23, 20	// ����(-Z��)
	};

	return boxMesh;
}
