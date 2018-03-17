
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};


cbuffer externalData : register(b0)
{

}


float4 main(VertexToPixel input) : SV_TARGET
{
	return input.color;
}