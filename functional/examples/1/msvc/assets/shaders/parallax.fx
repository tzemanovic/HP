float4x4 matWorld:WORLD<string UIWidget="None";>;
float4x4 matView:VIEW<string UIWidget="None";>;
float4x4 matProjection:PROJECTION<string UIWidget="None";>;

//Ambient
float4 ambientMaterialColour<
	string UIName="Ambient Material";
	string UIWidget="Color";
> =float4(1.0f,1.0f,1.0f,1.0f);

float4 diffuseMaterialColour<
	string UIName="Diffuse Material";
	string UIWidget="Color";
> =float4(1.0f,1.0f,1.0f,1.0f);

float4 specularMaterialColour<
	string UIName="Specular Material";
	string UIWidget="Color";
> =float4(1.0f,1.0f,1.0f,1.0f);

float4 ambientLightColour:AMBIENT;

float4 lightDirection:DIRECTION<
	string Object = "DirectionalLight";
>;

float4 diffuseLightColour:DIFFUSE<
	string Object = "DirectionalLight";
>;

float4 specularLightColour:SPECULAR<
	string Object = "DirectionalLight";
>;

float specularPower<
	string UIName="Specular Power";
	string UIWidget="Slider";
	float UIMin = 0.0;
	float UIMax = 100.0;
	float UIStep = 5.0;
> =25.0f;

float3 cameraPosition:CAMERAPOSITION<
	string Object ="Perspective";
>;

Texture2D diffuseMap;
bool useDiffuseTexture
<
string UIName="Use Diffuse Texture";
> = false;

Texture2D specularMap;
bool useSpecularTexture
<
string UIName="Use Specular Texture";
> = false;

Texture2D bumpMap;
bool useBumpTexture
<
string UIName="Use Bumpmapping";
> = false;

Texture2D heightMap;
bool useHeightTexture
<
string UIName="Use Parrallax";
> = false;

float scale=0.05;
float bias=0.01;
const float3x3 identityMatrix3x3 = {
	float3(1,0,0),
	float3(0,1,0),
	float3(0,0,1)
};
float2 textureRepeat = float2(1.0, 1.0);

SamplerState wrapSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_INPUT
{
	float4 pos:POSITION;
	float3 normal:NORMAL;
	float3 tangent:TANGENT;
	float2 texCoord:TEXCOORD0;
	float3 binormal:BINORMAL;
};

struct PS_INPUT
{
	float4 pos:SV_POSITION;
	float3 normal:NORMAL;
	float3 cameraDirection:VIEWDIR;
	float3 lightDir:LIGHTDIR;
	float2 texCoord:TEXCOORD0;
	float3 tangent:TANGENT;
	float3 binormal:BINORMAL;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	
	float4x4 matViewProjection = mul(matView, matProjection);
	float4x4 matWorldViewProjection = mul(matWorld, matViewProjection);
	float4 worldPos = mul(input.pos,matWorld);
	
	output.normal = normalize(mul(input.normal, matWorld));
	output.cameraDirection = mul(normalize(cameraPosition.xyz-worldPos.xyz), matWorld);	
	output.lightDir = normalize(lightDirection.xyz);		
	output.tangent = normalize(mul(input.tangent, matWorld));
	output.binormal = normalize(mul(input.binormal, matWorld));
	
	output.pos = mul(input.pos, matWorldViewProjection);

	output.texCoord = input.texCoord;
	return output;
}



float4 PS(PS_INPUT input):SV_TARGET
{
	float heightVal = heightMap.Sample(wrapSampler, input.texCoord).x;
	float height = scale * heightVal - bias;
	float2 texCoord = input.texCoord * textureRepeat + (height * input.cameraDirection.xy) * textureRepeat * useHeightTexture;
	
	float3 bumpMapSampled = ((2.0 * (bumpMap.Sample(wrapSampler, texCoord))) - 1.0) * useBumpTexture;
	
	float3 normal = normalize(input.normal + float3(bumpMapSampled.r, 0.0, bumpMapSampled.g));
	
	float3 lightDir = float3(input.lightDir.x, -input.lightDir.y, input.lightDir.z);
	
	float4 diffuseColour = diffuseMaterialColour * !useDiffuseTexture + diffuseMap.Sample(wrapSampler,texCoord)*  useDiffuseTexture;
	float4 specularColour = specularMaterialColour * !useSpecularTexture + specularMap.Sample(wrapSampler,texCoord) * useSpecularTexture;
		
	float diffuse = saturate(dot(normal, lightDir));
	
	float3 halfVec = normalize(lightDir + input.cameraDirection);
	float specular = pow(saturate(dot(normal, halfVec)), specularPower);
	
	return ((diffuseColour * ambientLightColour) +
		(diffuseColour * diffuseLightColour * diffuse) +
		(specularColour * specularLightColour * specular));
}

RasterizerState DisableCulling
{
    CullMode = BACK;
};

DepthStencilState EnableZBuffering
{
	DepthEnable = TRUE;
    DepthWriteMask = ALL;
    DepthFunc = LESS;
};

technique10 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
		SetRasterizerState(DisableCulling); 
		SetDepthStencilState(EnableZBuffering, 0);
	}
}