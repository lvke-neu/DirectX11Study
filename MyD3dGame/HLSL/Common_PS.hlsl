#include "Common.hlsli"

Texture2D g_Tex : register(t0);
SamplerState g_SamLinear : register(s0);
// 像素着色器
float4 PS(VertexOut pIn) : SV_Target
{
	
	// 标准化法向量
	pIn.normalW = normalize(pIn.normalW);
	// 顶点指向眼睛的向量
	float3 toEyeW = normalize(g_EyePosW - pIn.posW);
	// 初始化为0 
	float4 ambient, diffuse, spec;
	float4 A, D, S;
	ambient = diffuse = spec = A = D = S = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComputeDirectionalLight(g_Material, g_DirLight, pIn.normalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(g_Material, g_PointLight, pIn.posW, pIn.normalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputeSpotLight(g_Material, g_SpotLight, pIn.posW, pIn.normalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	float4 texColor = g_Tex.Sample(g_SamLinear, pIn.tex);
	float4 litColor = texColor * (ambient + diffuse) + spec;

	clip(texColor.a - 0.1f);

	litColor.a = g_Material.Diffuse.a;

	//return litColor;

	texColor.a = texColor.a* g_Material.Diffuse.a;
	return texColor;
}
