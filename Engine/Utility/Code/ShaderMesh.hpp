
#include "ShaderBase.hpp"

vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;
float		g_fPower;
float		g_fAlpha;

vector		g_vCamPos;
float		g_fDetail;
int			g_iHit = 0;
texture		g_BaseTexture;

// 하드웨어 스키닝 변수

Matrix		MatrixPalette[57];
int			NumBoneInfluence;


sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV * g_fDetail;

	//빛~
	vector		vLightDirInv = normalize(g_vLightDir) * -1.f;

	vector		vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	vWorldNormal = normalize(vWorldNormal);

	Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	vector		vReflect = reflect(normalize(g_vLightDir), vWorldNormal);

	vector		vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	vector		vLook = vWorldPos - g_vCamPos;

	Out.vSpecular = pow(max(dot(normalize(vLook * -1.f), normalize(vReflect)), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);
	return Out;
}

struct PS_IN
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = Out.vColor * In.vShade + In.vSpecular;
	Out.vColor.a = g_fAlpha;

	return Out;
}

struct VS_OUT_ALPHA
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

struct PS_IN_ALPHA
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
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

	//빛~
	vector		vLightDirInv = normalize(g_vLightDir) * -1.f;

	vector		vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	vWorldNormal = normalize(vWorldNormal);

	Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	vector		vReflect = reflect(normalize(g_vLightDir), vWorldNormal);

	vector		vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	vector		vLook = vWorldPos - g_vCamPos;

	Out.vSpecular = pow(max(dot(normalize(vLook * -1.f), normalize(vReflect)), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);

	return Out;
}

PS_OUT PS_MAIN_ALPHATEST(PS_IN_ALPHA In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor = Out.vColor * In.vShade;

	if(g_iHit == 1)
	{
		Out.vColor.rgb = 1.f;
	}
	// 타격시 
	return Out;
}

struct VS_INPUT_SKIN
{
	vector position : POSITION;
	vector normal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 weights : BLENDWEIGHT;
	float4 boneIndices : BLENDINDICES;
};

struct VS_OUTPUT_SKIN
{
	vector position : POSITION;
	float3 normal : NORMAL;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

VS_OUTPUT_SKIN VS_MAIN_SKIN(VS_INPUT_SKIN In)
{
	VS_OUTPUT_SKIN Out = (VS_OUTPUT_SKIN)0;

	Matrix		matWV, matVP, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matVP = mul(g_matView, g_matProj);
	matWVP = mul(matWV, g_matProj);

	vector p = vector(0.0f, 0.0f, 0.0f, 1.0f);
	vector norm = vector(0.0f, 0.0f, 0.0f, 0.0f);
	float lastWeight = 0.0f;
	int n = NumBoneInfluence-1;
	In.normal = normalize(In.normal);

	// 정점의 포지션과 노말을 블렌딩한다.
	for(int i = 0; i < n; ++i)
	{
		lastWeight += In.weights[i];
		p += In.weights[i] * mul(In.position, MatrixPalette[In.boneIndices[i]]);

		norm += In.weights[i] * mul(In.normal, MatrixPalette[In.boneIndices[i]]);
	}

	lastWeight = 1.0f - lastWeight;
	p += lastWeight * mul(In.position, MatrixPalette[In.boneIndices[n]]);

	norm += lastWeight * mul(In.normal, MatrixPalette[In.boneIndices[n]]);
	p.w = 1.0f;

	// 정점을 월드 공간으로 변환한다.
	vector posWorld = mul(p, matWVP);

	// 다시 스크린 공간으로 변환한다.
	Out.position = posWorld/*mul(posWorld, matVP)*/;

	// UV 좌표를 복사.
	Out.vTexUV = In.vTexUV;

	vector		vLightDirInv = normalize(g_vLightDir) * -1.f;

	vector		vWorldNormal = mul(vector(In.normal.xyz, 0.f), g_matWorld);
	vWorldNormal = normalize(vWorldNormal);

	Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	vector		vReflect = reflect(normalize(g_vLightDir), vWorldNormal);

	vector		vWorldPos = mul(vector(In.position.xyz, 1.f), g_matWorld);
	vector		vLook = vWorldPos - g_vCamPos;

	Out.vSpecular = pow(max(dot(normalize(vLook * -1.f), normalize(vReflect)), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);
	
	return Out;
}

struct PS_IN_SKIN
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT_SKIN
{
	vector		vColor : COLOR0;
};

PS_OUT_SKIN PS_MAIN_SKIN(PS_IN_SKIN In)
{
	PS_OUT_SKIN		Out = (PS_OUT_SKIN)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = Out.vColor * In.vShade + In.vSpecular;

	return Out;
}

struct VS_OUT_SKIN_ALPHA
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};


struct PS_IN_SKIN_ALPHA
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};
VS_OUT_SKIN_ALPHA VS_MAIN_SKIN_ALPHA(VS_INPUT_SKIN In)
{
	VS_OUT_ALPHA Out = (VS_OUT_ALPHA)0;

	Matrix		matWV, matVP, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matVP = mul(g_matView, g_matProj);
	matWVP = mul(matWV, g_matProj);

	vector p = vector(0.0f, 0.0f, 0.0f, 1.0f);
	vector norm = vector(0.0f, 0.0f, 0.0f, 0.0f);
	float lastWeight = 0.0f;
	int n = NumBoneInfluence-1;
	In.normal = normalize(In.normal);

	// 정점의 포지션과 노말을 블렌딩한다.
	for(int i = 0; i < n; ++i)
	{
		lastWeight += In.weights[i];
		p += In.weights[i] * mul(In.position, MatrixPalette[In.boneIndices[i]]);

		norm += In.weights[i] * mul(In.normal, MatrixPalette[In.boneIndices[i]]);
	}

	lastWeight = 1.0f - lastWeight;
	p += lastWeight * mul(In.position, MatrixPalette[In.boneIndices[n]]);

	norm += lastWeight * mul(In.normal, MatrixPalette[In.boneIndices[n]]);
	p.w = 1.0f;

	// 정점을 월드 공간으로 변환한다.
	vector posWorld = mul(p, matWVP);

	// 다시 스크린 공간으로 변환한다.
	Out.vPosition = posWorld/*mul(posWorld, matVP)*/;

	// UV 좌표를 복사.
	Out.vTexUV = In.vTexUV;

	//빛~
	vector		vLightDirInv = normalize(g_vLightDir) * -1.f;

	vector		vWorldNormal = mul(vector(In.normal.xyz, 0.f), g_matWorld);
	vWorldNormal = normalize(vWorldNormal);

	Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	vector		vReflect = reflect(normalize(g_vLightDir), vWorldNormal);

	vector		vWorldPos = mul(vector(In.position.xyz, 1.f), g_matWorld);
	vector		vLook = vWorldPos - g_vCamPos;

	Out.vSpecular = pow(max(dot(normalize(vLook * -1.f), normalize(vReflect)), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);

	return Out;
}

PS_OUT PS_MAIN_SKIN_ALPHA(PS_IN_SKIN_ALPHA In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = Out.vColor * In.vShade + In.vSpecular;

	return Out;
}

technique Default_Technique
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass AlphaTest
	{
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0x0000003f;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN_ALPHATEST();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHATEST();
	}

	pass HardWareSkinning
	{
		VertexShader = compile vs_3_0 VS_MAIN_SKIN();
		PixelShader = compile ps_3_0 PS_MAIN_SKIN();
	}
	pass HardWareSkinning_Alpha
	{
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0x0000003f;

		VertexShader = compile vs_3_0 VS_MAIN_SKIN_ALPHA();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHATEST();

	}
};