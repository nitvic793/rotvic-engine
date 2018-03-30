


struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer externalData : register(b0)
{
	DirectionalLight light1;
};


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





float4 main(VertexToPixel input) : SV_TARGET
{
	//return float4(0, 1, 0, 1);


	input.normal = normalize(input.normal);

//Light Calculations
float3 lightDir = -normalize(light1.Direction);
float dirNdotL1 = dot(input.normal, lightDir);
dirNdotL1 = saturate(dirNdotL1);



//float4 surfaceColor = DiffuseTexture.Sample(Sampler, input.uv);

return float4((dirNdotL1*light1.DiffuseColor)  + light1.AmbientColor);
//return float4(surfaceColor* ((dirNdotL1*light1.DiffuseColor )+ (dirNdotL2*light2.DiffuseColor) + light1.AmbientColor));

}