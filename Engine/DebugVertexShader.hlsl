
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexInput
{
	float3 position		: POSITION;
	float4 color		: COLOR;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

VertexToPixel main(VertexInput input)
{
	VertexToPixel output;
	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.color = input.color;

	return output;
}