#include "Common.hlsli"

Texture2D g_Tex : register(t0);
SamplerState g_SamLinear : register(s0);
// 像素着色器
float4 PS(VertexOut pIn) : SV_Target
{
	float4 texColor = g_Tex.Sample(g_SamLinear, pIn.tex);
	clip(texColor.a - 0.1f);

	// 标准化法向量
	pIn.normalW = normalize(pIn.normalW);
	// 顶点指向眼睛的向量
	float3 toEyeW = normalize(g_EyePosW - pIn.posW);
	// 初始化为0 
	float4 ambient, diffuse, spec;
	float4 A, D, S;
	ambient = diffuse = spec = A = D = S = float4(0.0f, 0.0f, 0.0f, 0.0f);


	DirectionalLight dirLight = g_DirLight;
	[flatten]
	if (g_IsReflection)
	{
		dirLight.Direction = mul(dirLight.Direction, (float3x3) (g_Reflection));
	}
	ComputeDirectionalLight(g_Material, g_DirLight, pIn.normalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;


	PointLight pointLight;
	pointLight = g_PointLight;
	[flatten]
	if (g_IsReflection)
	{
		pointLight.Position = (float3) mul(float4(pointLight.Position, 1.0f), g_Reflection);
	}
	ComputePointLight(g_Material, pointLight, pIn.posW, pIn.normalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;


	SpotLight spotLight;
	spotLight = g_SpotLight;
	[flatten]
	if (g_IsReflection)
	{
		spotLight.Position = (float3) mul(float4(spotLight.Position, 1.0f), g_Reflection);
		spotLight.Direction = mul(spotLight.Direction, (float3x3) g_Reflection);
	}
	ComputeSpotLight(g_Material, spotLight, pIn.posW, pIn.normalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;


	float4 litColor = texColor * (ambient + diffuse) + spec;
	litColor.a = g_Material.Diffuse.a;

	return litColor;

	/*texColor.a = texColor.a* g_Material.Diffuse.a;
	return texColor;*/

	
}
