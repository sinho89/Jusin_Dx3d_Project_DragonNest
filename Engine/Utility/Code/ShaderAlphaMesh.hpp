#include "ShaderBase.hpp"			// ÀÌ°Å ¾È¾¸¿ä

struct VS_OUT_ALPHA
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	float2		vTexUV : TEXCOORD0;
};

struct PS_IN_ALPHA
{
	vector		vShade : COLOR0;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT_ALPHA VS_MAIN_ALPHATEST(VS_IN In)
{
	VS_OUT_ALPHA		Out = (VS_OUT_ALPHA)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	//ºû~
	vector		vLightDirInv = normalize(g_vLightDir) * -1.f;

	vector		vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	vWorldNormal = normalize(vWorldNormal);

	Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	return Out;
}

PS_OUT PS_MAIN_ALPHATEST(PS_IN_ALPHA In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = Out.vColor * In.vShade;

	return Out;
}

technique Default_Technique
{
	pass AlphaTest
	{
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0x0000003f;

		VertexShader = compile vs_3_0 VS_MAIN_ALPHATEST();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHATEST();
	}
};