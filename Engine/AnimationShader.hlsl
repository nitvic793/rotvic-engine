
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct Bones
{
	matrix BoneTransform;
	matrix InvBoneTransform;
};


cbuffer bones : register(b1)
{
	Bones bones[20];
}


struct VertexShaderInput
{

	float4 position		: POSITION;     // XYZ position
	float3 normal       : NORMAL;
	//float2 uv           : TEXCOORD;
	float4 boneid		: BONEID;
	float4 weight		: WEIGHT;
	//float4 color		: COLOR;        // RGBA color

};


struct VertexToPixel
{

	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 normal       : NORMAL;
	//float2 uv           : TEXCOORD;
	//float4 color		: COLOR;        // RGBA color
};



VertexToPixel main(VertexShaderInput input)
{

	VertexToPixel output;


	matrix worldViewProj = mul(mul(world, view), projection);

	matrix bonetransform = 0;



	if (input.boneid.x != -1)
	{
		bonetransform = mul(mul(bones[input.boneid.x].BoneTransform, input.weight.x), bones[input.boneid.x].InvBoneTransform);
	}
	if (input.boneid.y != -1)
	{
		bonetransform += mul(mul(bones[input.boneid.y].BoneTransform, input.weight.y), bones[input.boneid.y].InvBoneTransform);
	}
	if (input.boneid.z != -1)
	{
		bonetransform += mul(mul(bones[input.boneid.z].BoneTransform, input.weight.z), bones[input.boneid.z].InvBoneTransform);
	}
	if (input.boneid.w != -1)
	{
		bonetransform += mul(mul(bones[input.boneid.w].BoneTransform, input.weight.w), bones[input.boneid.w].InvBoneTransform);
	}


	output.position = mul(mul(bonetransform, input.position), worldViewProj);
	output.normal = mul(mul(bonetransform, input.normal), (float3x3)world);
	return output;
}