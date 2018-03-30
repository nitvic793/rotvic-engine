


struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer externalData : register(b0)
{
	DirectionalLight light1;
	DirectionalLight light2;
};
/*
Texture2D DiffuseTexture  : register(t0);
SamplerState Sampler : register(s0);

*/

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal       : NORMAL;
	//float2 uv           : TEXCOORD;

	//float4 color		: COLOR;
};






// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	//return float4(0, 1, 0, 1);

	
	input.normal = normalize(input.normal);

	//Light Calculations
	float3 lightDir = -normalize(light1.Direction);
	float dirNdotL1 = dot(input.normal, lightDir);
	dirNdotL1 = saturate(dirNdotL1);
	
	lightDir = -normalize(light2.Direction);
	float dirNdotL2 = dot(input.normal, lightDir);
	dirNdotL2 = saturate(dirNdotL2);

	//float4 surfaceColor = DiffuseTexture.Sample(Sampler, input.uv);

	return float4((dirNdotL1*light1.DiffuseColor) + (dirNdotL2*light2.DiffuseColor) + light1.AmbientColor);
	//return float4(surfaceColor* ((dirNdotL1*light1.DiffuseColor )+ (dirNdotL2*light2.DiffuseColor) + light1.AmbientColor));
	
}