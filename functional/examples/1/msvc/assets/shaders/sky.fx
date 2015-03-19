float4x4 matView:VIEW;
float4x4 matProjection:PROJECTION;
TextureCube envMap;

SamplerState wrapSampler
{
    Filter=MIN_MAG_MIP_LINEAR;
    AddressU=Clamp;
    AddressV=Clamp;
};

struct VS_INPUT
{
	float4 pos:POSITION;
};

struct PS_INPUT
{
	float4 pos:SV_POSITION;
	float3 texCoord:TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output=(PS_INPUT)0;
	// cancel out the translation
	float4x4 matViewAdj = {matView._11,matView._12,matView._13,matView._14,
		matView._21,matView._22,matView._23,matView._24,
		matView._31,matView._32,matView._33,matView._34,
		0.0f,0.0f,0.0f,matView._44};
	float4x4 matViewProjection=mul(matViewAdj,matProjection);
	output.pos=mul(input.pos,matViewProjection);
	output.texCoord=normalize(input.pos.xyz);
	return output;
}

float4 PS(PS_INPUT input):SV_TARGET
{
	return envMap.Sample(wrapSampler,input.texCoord);
}

RasterizerState DisableCulling
{
    CullMode = NONE;
};

DepthStencilState DisableZBuffering
{
	DepthEnable = FALSE;
};

technique10 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0,  PS() ) );
		SetRasterizerState(DisableCulling);
		SetDepthStencilState(DisableZBuffering,0);
	}
}