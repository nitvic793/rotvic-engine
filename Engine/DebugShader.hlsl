
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: POSITION;
	float3 tangent		: TANGENT;
};


cbuffer externalData : register(b0)
{

}


float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(1,1,1,1);
}