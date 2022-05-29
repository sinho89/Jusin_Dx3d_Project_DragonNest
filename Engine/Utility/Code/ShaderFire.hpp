sampler smp0 : register(s0);
sampler smp1 : register(s1);
sampler smp2 : register(s2);
sampler smp3 : register(s3);


struct SvsOut					// for output vertex shader
{
	float4 Pos : POSITION;
	float4 Dif : COLOR0;
	float2 Tex : TEXCOORD0;
};


float4x4	m_mtWVP;			// World * View * Projection Matrix;

static float m_dstR	= 10.7;		// distortion range for x and y


////////////////////////////////////////////////////////////////////////////
// 화염 효과

float	g_HeatHaze = 0.05;

SvsOut VtxPrcDst( float3 Pos : POSITION
				 , float2 Tex : TEXCOORD0)
{
	SvsOut Out=(SvsOut)0;

	Out.Pos = float4(Pos,1);
	Out.Tex = Tex;

	return Out;
}

float4 PxlPrcDst(SvsOut In) : COLOR0
{
	float4 Out=0;

	float4	Pert = tex2D(smp1, In.Tex);

	float	x = In.Tex.x + (Pert.x-0.5) * g_HeatHaze;
	float	y = In.Tex.y + (Pert.y-0.5) * g_HeatHaze;

	Out = tex2D(smp0, float2(x,y));

	Out *= 1.5f;

	return Out;
}


////////////////////////////////////////////////////////////////////////////
// 변위에 상관 없는 오브젝트

SvsOut VtxPrcObj( float3 Pos : POSITION
				 , float4 Dif : COLOR0
				 , float2 Tex : TEXCOORD0)
{
	SvsOut Out=(SvsOut)0;

	Out.Pos = mul(float4(Pos,1), m_mtWVP);
	Out.Tex = Tex;

	return Out;
}

float4 PxlPrcObj(SvsOut In) : COLOR0
{
	float4	Out={1,1,1,1};

	Out = tex2D(smp0, In.Tex);

	Out.r=0.5f;
	Out.g=0.5f;
	Out.b=0.5f;

	return Out;
}


////////////////////////////////////////////////////////////////////////////
// 변위에 사용할 Particle

SvsOut VtxPrcPtc( float3 Pos : POSITION
				 , float4 Dif : COLOR0
				 , float2 Tex : TEXCOORD0)
{
	SvsOut Out=(SvsOut)0;

	float4	Tpos= mul(float4(Pos,1), m_mtWVP);

	Out.Dif = Dif;
	Out.Tex = Tex;

	Tpos *=m_dstR;

	Out.Pos = Tpos;

	return Out;
}

float4 PxlPrcPtc(SvsOut In) : COLOR0
{
	float4	Out={1,1,1,1};
	float4  t1= In.Dif;

	Out = tex2D(smp0, In.Tex);
	Out.w *= t1.w;

	return Out;
}



technique Tech
{
	pass P0		
	{
		VertexShader = compile vs_1_1 VtxPrcDst();
		PixelShader  = compile ps_2_0 PxlPrcDst();
	}

	pass P1		
	{
		VertexShader = compile vs_1_1 VtxPrcObj();
		PixelShader  = compile ps_2_0 PxlPrcObj();
	}

	pass P2		
	{
		VertexShader = compile vs_1_1 VtxPrcPtc();
		PixelShader  = compile ps_2_0 PxlPrcPtc();
	}
};

